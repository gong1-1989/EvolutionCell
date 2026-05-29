# EvolutionCell — Config 目录说明

本目录保存 EvolutionCell 项目的运行时配置文件（JSON 格式）。每个文件对应一类配置：实体属性、系统规则、AI 行为、UI 提示等。

编辑前请先阅读「编辑规范」与「验证方法」部分，避免破坏运行时依赖关系（例如 ID 关联、权重之和等）。

## 文件列表（说明）

- 目录 1：【全局通用】(4 个)
- global_rule.json 全局基础铁律、通用公式、异常规则
- ai_runtime.json AI 运行参数、帧率、行为判定、容错规则
- lineage_save.json 谱系存档、轮回继承、存档规则
- difficulty_mode.json 难度模式、游玩模式参数

- 目录 2：【生物核心】(8 个)
- cell_base.json 细胞基础属性
- organelle_diff.json 细胞器 & 分化规则
- gene_template.json 基因实体数据模板
- gene_library.json 全局基因库
- gene_mutation.json 突变、水平基因转移规则
- inter_species.json 种间关系规则
- group_biofilm.json 群体感应、生物膜、群落阵型
- evolution_style.json 演化流派配置
- metabolism.json 代谢通路配置

- 目录 3：【生态环境】(4 个)
- environment_layer.json 8 大层级、环境因子、演替阶段
- migrate_matter.json 跨层迁徙、物质 / 环境传导、演替联动
- ecology_event.json 随机生态事件
- supply_item.json 局内道具

- 目录 4：【交互 & 表现】(3 个)
- chemotaxis_signal.json 标记、趋化信号配置
- ui_style_tips.json UI 样式、弹窗、提示规则
- visual_effect.json 视觉标识、动画、特效触发规则

## 编辑规范

- 格式：所有文件必须为有效 JSON（UTF-8，无 BOM），禁止使用注释或尾随逗号。
- 字段命名：采用小写下划线风格（snake_case），新增字段请保持向后兼容。
- ID 规则：实体类配置请使用唯一整数 `id`，不要改变已存在 ID 的含义。
- 权重/概率：概率类字段应保证值在 [0,1]。若为权重数组，注意总和或文档中注明的归一化规则。
- 变更流程：重大模式或字段变更前请先创建 JSON Schema 并通过团队评审。

## 常见字段示例

示例：'cell_base.json' 中项的典型结构

'''
{
	"id": 101,
	"name": "基本细胞",
	"hp": 100,
	"speed": 1.2,
	"spore_trigger_prob": 0.03
}
'''

说明：'spore_trigger_prob' 为孢子触发概率，取值范围 0 到 1。

## 验证与测试

建议在保存前使用 JSON 校验工具：

# 使用 Python 校验（跨平台）
python -m json.tool path/to/file.json

# 使用 jq 在命令行验证并格式化
jq . path/to/file.json > /dev/null

在 Windows PowerShell 中，也可以使用：

Get-Content path\\to\\file.json -Raw | ConvertFrom-Json > $null

此外建议为关键配置建立 JSON Schema 并使用自动化 CI 校验。

## 版本与变更记录

- 请在对结构性字段（IDs、主键、重要命名）进行变更时记录在本处，示例：

- 2026-05-28：为 'cell_base.json' 添加字段 'spore_trigger_prob'；为 'environment_factor.json' 添加古菌层 'archaea_cycle' 字段。

## 注意事项

- 修改配置后请确认游戏运行端或测试环境能正确加载并通过单元测试。
- 避免直接在运行服务器上修改生产文件，先在分支与测试环境验证。