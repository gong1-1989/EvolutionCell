#include "PlayerCell.h"

PlayerCell::PlayerCell()
    : m_pos(0, 0)
    , m_size(ConfigReader::getInstance().getDouble("player_setting", "init_size", 20))
    , m_lawType(GameGlobal::LAW_FISSION)
    , m_decomposeLv(GameGlobal::DECOMPOSE_NONE)
    , m_currentDecomposeRisk(0)
    , m_geneRejectValue(0)
    , m_mimicState(GameGlobal::MIMIC_NONE)
    ,m_mimicEndTime(0)
    ,m_mimicCoolDown(3000)
    ,m_geneStableValue(100)
    ,m_geneStableState(GameGlobal::STABLE_SAFE)
{
    m_unlockGene.clear();
    m_symbiosisList.clear();
    m_historyNodeList.clear();
}

// ===================== 基础移动 =====================
void PlayerCell::move(bool keyW, bool keyA, bool keyS, bool keyD, int canvasW, int canvasH)
{
    qreal speed = ConfigReader::getInstance().getDouble("player_setting", "move_speed", 2.8);
    qreal dx = 0, dy = 0;

    if (keyW) dy -= speed;
    if (keyS) dy += speed;
    if (keyA) dx -= speed;
    if (keyD) dx += speed;

    // 归一化，防止斜向加速
    if (dx != 0 && dy != 0)
    {
        double ratio = 1.0 / sqrt(dx * dx + dy * dy);
        dx *= ratio * speed;
        dy *= ratio * speed;
    }

    // 位置更新
    m_pos.rx() += dx;
    m_pos.ry() += dy;

    // 边界限制
    int halfSize = m_size / 2;
    if (m_pos.x() - halfSize < 0) m_pos.setX(halfSize);
    if (m_pos.x() + halfSize > canvasW) m_pos.setX(canvasW - halfSize);
    if (m_pos.y() - halfSize < 0) m_pos.setY(halfSize);
    if (m_pos.y() + halfSize > canvasH) m_pos.setY(canvasH - halfSize);
}

void PlayerCell::grow(int addSize, GameGlobal::MonsterType type)
{
    int maxSize = ConfigReader::getInstance().getInt("player_setting", "max_size", 150);
    m_size += addSize;
    if (m_size > maxSize)
        m_size = maxSize;
}

// ===================== 躯体解构系统 =====================
void PlayerCell::executeDecompose(GameGlobal::DecomposeLevel lv)
{
    if (m_decomposeLv >= lv)
        return;

    int risk = GameGlobal::getDecomposeRisk(lv);
    addDecomposeRisk(risk);
    m_decomposeLv = lv;
}

GameGlobal::DecomposeLevel PlayerCell::getDecomposeLevel() const
{
    return m_decomposeLv;
}

int PlayerCell::getDecomposeRisk() const
{
    return m_currentDecomposeRisk;
}

void PlayerCell::addDecomposeRisk(int val)
{
    m_currentDecomposeRisk += val;
}

// ===================== 基因系统 =====================
bool PlayerCell::unlockGene(GameGlobal::GeneType type)
{
    // 重复解锁直接返回
    for (auto t : m_unlockGene)
    {
        if (t == type)
            return false;
    }
    m_unlockGene.append(type);
    // 解锁新基因自动记录演化节点
    recordCurrentEvolveNode();
    return true;
}

const QList<GameGlobal::GeneType>& PlayerCell::getUnlockedGene() const
{
    return m_unlockGene;
}

//====================== 拟态伪装系统实现 ========================
void PlayerCell::startMimic(GameGlobal::MimicState mimicType, qint64 lastTime){
    //拟态冷却中无法启动
    if(isInMimc()) return;
    //设置拟态状态
    m_mimicState=mimicType;
    //计算结束时间
    m_mimicEndTime=QDateTime::currentMSecsSinceEpoch()+lastTime;
}

void PlayerCell::stopMimic(){
    m_mimicState=GameGlobal::MIMIC_NONE;
    m_mimicEndTime=0;
}

void PlayerCell::updateMimic(qint64 nowTime){
    //无拟态直接返回
    if(m_mimicState==GameGlobal::MIMIC_NONE) return;
    //拟态超时自动关闭
    if(nowTime>=m_mimicEndTime) stopMimic();
}

GameGlobal::MimicState PlayerCell::getMimicState()const{
    return m_mimicState;
}

bool PlayerCell::isInMimc()const{
    return m_mimicState!=GameGlobal::MIMIC_NONE&&m_mimicEndTime>QDateTime::currentMSecsSinceEpoch();
}

//==================== 基因熵变系统实现 ======================
bool PlayerCell::triggerGeneChao(){
    //稳定度不足，无法触发
    if(m_geneRejectValue<30) return false;
    //扣除稳定度（熵变代价）
    m_geneRejectValue-=30;
    //随机触发属性波动（核心熵变逻辑）
    int randVal=RandomUtil::randInt(1,100);
    if(randVal<=40){
        //正向畸变：体系变大
        grow(5,GameGlobal::NORMAL);
    }else if(randVal<=70){
        //中性畸变：无变化
    }else{
        //负向畸变：体型减少
        m_size=qMax(ConfigReader::getInstance().getInt("player_setting", "init_size", 20),m_size-5);
    }
    //更新基因稳定状态
    if(m_geneStableValue>=70) m_geneStableState=GameGlobal::STABLE_SAFE;
    else if(m_geneStableValue>=30) m_geneStableState=GameGlobal::STABLE_WARN;
    else m_geneStableState=GameGlobal::STABLE_DANGER;
    return true;
}

void PlayerCell::repairGeneStable(int addVal){
    m_geneStableValue=qMin(100,m_geneStableValue+addVal);
    //重新计算基因稳定状态
    if(m_geneStableValue>=70) m_geneStableState=GameGlobal::STABLE_SAFE;
    else if(m_geneStableValue>=30) m_geneStableState=GameGlobal::STABLE_WARN;
    else m_geneStableState=GameGlobal::STABLE_DANGER;
}

int PlayerCell::getGeneStableValue()const{
    return m_geneStableValue;
}

GameGlobal::GeneStableState PlayerCell::getGeneStableState()const{
    return m_geneStableState;
}

// ===================== 共生体系 =====================
bool PlayerCell::addSymbiosis(const SymbiosisCell& cell)
{
    int maxCnt = ConfigReader::getInstance().getInt("symbiosis_setting", "max_count", 6);
    if (m_symbiosisList.size() >= maxCnt)
        return false;

    m_symbiosisList.append(cell);
    addGeneReject( ConfigReader::getInstance().getInt("symbiosis_setting", "single_reject", 8));
    return true;
}

void PlayerCell::clearExpiredSymbiosis(qint64 nowTime)
{
    // 倒序遍历，删除元素不打乱下标
    for (int i = m_symbiosisList.size() - 1; i >= 0; --i)
    {
        if (m_symbiosisList[i].isTempExpired(nowTime))
        {
            subGeneReject(ConfigReader::getInstance().getInt("symbiosis_setting", "single_reject", 8));
            m_symbiosisList.removeAt(i);
        }
    }
}

void PlayerCell::clearAllSymbiosis()
{
    m_symbiosisList.clear();
    m_geneRejectValue = 0;
}

void PlayerCell::updateSymbiosisFollow(qreal playerX, qreal playerY, const QList<MonsterCell> &monsterList)
{
    qreal range = ConfigReader::getInstance().getDouble("symbiosis_setting", "follow_range", 150.0);
    for (auto& cell : m_symbiosisList)
    {
        cell.update(playerX, playerY, range, monsterList);
    }
}

int PlayerCell::getSymbiosisCount() const
{
    return m_symbiosisList.size();
}

const QList<SymbiosisCell>& PlayerCell::getSymbiosisList() const
{
    return m_symbiosisList;
}

int PlayerCell::getGeneRejectValue() const
{
    return m_geneRejectValue;
}

void PlayerCell::setGeneRejectValue(int val)
{
    m_geneRejectValue = val;
}

void PlayerCell::addGeneReject(int val)
{
    m_geneRejectValue += val;
}

void PlayerCell::subGeneReject(int val)
{
    m_geneRejectValue -= val;
    if (m_geneRejectValue < 0)
        m_geneRejectValue = 0;
}

GameGlobal::RejectLevel PlayerCell::getCurrentRejectLevel() const
{
    int warn = ConfigReader::getInstance().getInt("symbiosis_setting", "warn_threshold", 25);
    int danger =  ConfigReader::getInstance().getInt("symbiosis_setting", "danger_threshold", 50);
    if (m_geneRejectValue >= danger) return GameGlobal::REJECT_DANGER;
    if (m_geneRejectValue >= warn)  return GameGlobal::REJECT_WARNING;
    return GameGlobal::REJECT_SAFE;
}

// ===================== 时空回溯 历史节点 =====================
void PlayerCell::recordCurrentEvolveNode()
{
    int maxNode = ConfigReader::getInstance().getInt("rollback_ghost_setting", "max_history_node", 8);
    if (m_historyNodeList.size() >= maxNode)
        m_historyNodeList.removeFirst();

    GameGlobal::EvolveHistoryNode node;
    node.posX           = m_pos.x();
    node.posY           = m_pos.y();
    node.bodySize       = m_size;
    node.currentLaw     = m_lawType;
    node.decomposeLv    = m_decomposeLv;
    node.decomposeRisk  = m_currentDecomposeRisk;
    node.geneRejectValue= m_geneRejectValue;

    node.unlockGeneType.clear();
    for (auto t : m_unlockGene)
        node.unlockGeneType.append((int)t);

    m_historyNodeList.append(node);
}

bool PlayerCell::rollbackToLastNode()
{
    if (m_historyNodeList.isEmpty())
        return false;

    int costRisk =ConfigReader::getInstance().getInt("rollback_ghost_setting", "rollback_cost_risk", 10);
    int costReject = ConfigReader::getInstance().getInt("rollback_ghost_setting", "rollback_cost_reject", 15);
    if (m_currentDecomposeRisk < costRisk || m_geneRejectValue < costReject)
        return false;

    // 扣除回溯代价
    m_currentDecomposeRisk -= costRisk;
    m_geneRejectValue = qMax( 0 , m_geneRejectValue - costReject);

    // 恢复最后一个节点
    GameGlobal::EvolveHistoryNode node = m_historyNodeList.takeLast();
    m_pos.setX(node.posX);
    m_pos.setY(node.posY);
    m_size = node.bodySize;
    m_lawType = node.currentLaw;
    m_decomposeLv = node.decomposeLv;
    m_currentDecomposeRisk = node.decomposeRisk;
    m_geneRejectValue = node.geneRejectValue;

    // 恢复基因
    m_unlockGene.clear();
    for (int t : node.unlockGeneType)
        unlockGene(static_cast<GameGlobal::GeneType>(t));

    return true;
}

bool PlayerCell::rollbackToAssignNode(int index)
{
    if (m_historyNodeList.isEmpty() || index < 0 || index >= m_historyNodeList.size())
        return false;

    int costRisk =ConfigReader::getInstance().getInt("rollback_ghost_setting", "rollback_cost_risk", 10);
    int costReject = ConfigReader::getInstance().getInt("rollback_ghost_setting", "rollback_cost_reject", 15);
    if (m_currentDecomposeRisk < costRisk || m_geneRejectValue < costReject)
        return false;

    m_currentDecomposeRisk -= costRisk;
    m_geneRejectValue -= costReject;

    GameGlobal::EvolveHistoryNode node = m_historyNodeList.at(index);
    // 恢复全量状态
    m_pos.setX(node.posX);
    m_pos.setY(node.posY);
    m_size = node.bodySize;
    m_lawType = node.currentLaw;
    m_decomposeLv = node.decomposeLv;
    m_currentDecomposeRisk = node.decomposeRisk;
    m_geneRejectValue = node.geneRejectValue;

    m_unlockGene.clear();
    for (int t : node.unlockGeneType)
        unlockGene(static_cast<GameGlobal::GeneType>(t));

    // 截断后续节点，改写演化路线
    while (m_historyNodeList.size() > index + 1)
        m_historyNodeList.removeLast();

    return true;
}

int PlayerCell::getHistoryNodeCount() const
{
    return m_historyNodeList.size();
}

GameGlobal::EvolveHistoryNode PlayerCell::getHistoryNodeByIndex(int index) const
{
    GameGlobal::EvolveHistoryNode empty;
    if (index < 0 || index >= m_historyNodeList.size())
        return empty;
    return m_historyNodeList.at(index);
}

// ===================== 基础属性接口 =====================
qreal PlayerCell::getX() const { return m_pos.x(); }
qreal PlayerCell::getY() const { return m_pos.y(); }
void PlayerCell::setPos(qreal x, qreal y) { m_pos.setX(x); m_pos.setY(y); }
int PlayerCell::getSize() const { return m_size; }
GameGlobal::LifeLaw PlayerCell::getLawType() const { return m_lawType; }
void PlayerCell::setLawType(GameGlobal::LifeLaw law) { m_lawType = law; }

// ===================== 全量序列化 =====================
QJsonObject PlayerCell::toJson() const
{
    QJsonObject obj;
    obj["pos_x"]        = m_pos.x();
    obj["pos_y"]        = m_pos.y();
    obj["size"]         = m_size;
    obj["law_type"]     = (int)m_lawType;
    obj["decompose_lv"] = (int)m_decomposeLv;
    obj["decompose_risk"]= m_currentDecomposeRisk;
    obj["gene_reject"]  = m_geneRejectValue;

    // 基因列表
    QJsonArray geneArr;
    for (auto t : m_unlockGene)
        geneArr.append((int)t);
    obj["unlock_gene"] = geneArr;

    // 历史节点（简化：仅序列化快照核心数据）
    QJsonArray nodeArr;
    for (const auto& n : m_historyNodeList)
    {
        QJsonObject nObj;
        nObj["pos_x"] = n.posX;
        nObj["pos_y"] = n.posY;
        nObj["size"] = n.bodySize;
        nObj["law"] = (int)n.currentLaw;
        nObj["decompose"] = (int)n.decomposeLv;
        nObj["risk"] = n.decomposeRisk;
        nObj["reject"] = n.geneRejectValue;

        QJsonArray gArr;
        for (int t : n.unlockGeneType) gArr.append(t);
        nObj["gene"] = gArr;
        nodeArr.append(nObj);
    }
    obj["history_node"] = nodeArr;
    return obj;
}

void PlayerCell::fromJson(const QJsonObject& obj)
{
    m_pos.setX(obj["pos_x"].toDouble(0));
    m_pos.setY(obj["pos_y"].toDouble(0));
    m_size          = obj["size"].toInt(20);
    m_lawType       = static_cast<GameGlobal::LifeLaw>(obj["law_type"].toInt(0));
    m_decomposeLv   = static_cast<GameGlobal::DecomposeLevel>(obj["decompose_lv"].toInt(0));
    m_currentDecomposeRisk = obj["decompose_risk"].toInt(0);
    m_geneRejectValue = obj["gene_reject"].toInt(0);

    // 恢复基因
    m_unlockGene.clear();
    QJsonArray geneArr = obj["unlock_gene"].toArray();
    for (auto v : geneArr)
        unlockGene(static_cast<GameGlobal::GeneType>(v.toInt(0)));

    // 恢复历史节点
    m_historyNodeList.clear();
    QJsonArray nodeArr = obj["history_node"].toArray();
    for (auto nVal : nodeArr)
    {
        QJsonObject nObj = nVal.toObject();
        GameGlobal::EvolveHistoryNode node;
        node.posX = nObj["pos_x"].toDouble();
        node.posY = nObj["pos_y"].toDouble();
        node.bodySize = nObj["size"].toInt();
        node.currentLaw = static_cast<GameGlobal::LifeLaw>(nObj["law"].toInt());
        node.decomposeLv = static_cast<GameGlobal::DecomposeLevel>(nObj["decompose"].toInt());
        node.decomposeRisk = nObj["risk"].toInt();
        node.geneRejectValue = nObj["reject"].toInt();

        QJsonArray gArr = nObj["gene"].toArray();
        node.unlockGeneType.clear();
        for (auto g : gArr) node.unlockGeneType.append(g.toInt());
        m_historyNodeList.append(node);
    }
}