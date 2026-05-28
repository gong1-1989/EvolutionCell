# EvolutionCell — Config 目录说明

本目录保存 EvolutionCell 项目的运行时配置文件（JSON 格式）。每个文件对应一类配置：实体属性、系统规则、AI 行为、UI 提示等。

编辑前请先阅读「编辑规范」与「验证方法」部分，避免破坏运行时依赖关系（例如 ID 关联、权重之和等）。

## 文件列表（说明）

- `cell_base.json`：细胞基础属性（新增字段：孢子触发概率）。关键字段：`id`、`name`、`hp`、`speed`、`spore_trigger_prob`。
- `organelle_diff.json`：细胞器分化与重构规则。描述分化路径、触发条件和成本。
- `environment_factor.json`：8 大层级环境配置（新增：古菌层周期字段）。包含层级 ID、温度、营养系数、周期性事件参数。
- `gene_mutation.json`：基因突变配置。包含突变概率、突变效果描述和适应度影响。
- `inter_species.json`：种间关系配置。定义捕食、互利、竞争等关系矩阵与权重。
- `group_sense_biofilm.json`：群体感应与生物膜配置。包含阈值、信号扩散半径、耐受系数。
- `metabolism.json`：代谢通路配置。描述能量输入/输出、酶系效率与中间产物。
- `evolution_style.json`：演化流派配置（含孢子动画标识）。用于区分不同演化策略与视觉表现。
- `ecology_event.json`：随机生态事件配置。事件触发概率、持续时间与影响范围。
- `supply_item.json`：局内道具/物品配置。道具 ID、效果、冷却与获得方式。
- `lineage_tree.json`：谱系轮回与奖励配置。血脉加成、回溯规则、奖励分配。
- `ai_behavior.json`：AI 行为与判定阈值（含视觉/弹窗触发、结束判定）。行为树或权重表。
- `ui_tips.json`：UI 提示、弹窗样式与文案配置。
- `chemotaxis_signal.json`：趋化信号与区域标记配置。信号类型、衰减、优先级。
- `migrate_rule.json`：跨层迁徙与过渡带规则。迁徙条件、成功率与代价。

## 编辑规范

- 格式：所有文件必须为有效 JSON（UTF-8，无 BOM），禁止使用注释或尾随逗号。
- 字段命名：采用小写下划线风格（snake_case），新增字段请保持向后兼容。
- ID 规则：实体类配置请使用唯一整数 `id`，不要改变已存在 ID 的含义。
- 权重/概率：概率类字段应保证值在 [0,1]。若为权重数组，注意总和或文档中注明的归一化规则。
- 变更流程：重大模式或字段变更前请先创建 JSON Schema 并通过团队评审。

## 常见字段示例

示例：`cell_base.json` 中项的典型结构

```
{
	"id": 101,
	"name": "基本细胞",
	"hp": 100,
	"speed": 1.2,
	"spore_trigger_prob": 0.03
}
```

说明：`spore_trigger_prob` 为孢子触发概率，取值范围 0 到 1。

## 验证与测试

建议在保存前使用 JSON 校验工具：

```
# 使用 Python 校验（跨平台）
python -m json.tool path/to/file.json

# 使用 jq 在命令行验证并格式化
jq . path/to/file.json > /dev/null
```

在 Windows PowerShell 中，也可以使用：

```
Get-Content path\\to\\file.json -Raw | ConvertFrom-Json > $null
```

此外建议为关键配置建立 JSON Schema 并使用自动化 CI 校验。

## 版本与变更记录

- 请在对结构性字段（IDs、主键、重要命名）进行变更时记录在本处，示例：

- 2026-05-28：为 `cell_base.json` 添加字段 `spore_trigger_prob`；为 `environment_factor.json` 添加古菌层 `archaea_cycle` 字段。

## 注意事项

- 修改配置后请确认游戏运行端或测试环境能正确加载并通过单元测试。
- 避免直接在运行服务器上修改生产文件，先在分支与测试环境验证。