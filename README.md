CellEra/
├─ CMakeLists.txt               # 根目录构建脚本（主EXE）
├─ main.cpp                     # 程序入口
├─ Res/                         # 全局资源（所有配置/文案/美术/音频）
│  ├─ Art/                      # 像素美术、动画、图标
│  ├─ Audio/                    # BGM、音效
│  ├─ Config/                   # 15份标准JSON配置（策划第十一章）
│  └─ Text/                     # 9份标准TXT文案（策划第十二章）
├─ Save/                        # 轮回存档目录
├─ Plugins/                     # 所有动态DLL插件源码+编译输出
│  ├─ CMakeLists.txt            # 插件总构建脚本
│  ├─ EcoPlugin/                # 生态拓展插件
│  ├─ StylePlugin/              # 演化流派+古菌DLC插件
│  ├─ GroupEventPlugin/         # 群落+生态事件插件
│  ├─ GameModePlugin/           # 游玩模式+谱系拓展插件
│  ├─ UiSciencePlugin/          # UI+科普弹窗+预警插件
│  ├─ InteractionPlugin/        # 交互业务插件（新增）
│  ├─ AudioPlugin/              # 音频插件（新增）
│  └─ ParticlePlugin/           # 粒子特效插件（新增）
├─ src/                         # 静态内核源码（编译进主EXE）
│  ├─ Global/                   # 全局枚举、结构体、公式、工具
│  ├─ PluginInterface/          # 插件统一抽象接口（Qt插件标准）
│  ├─ PluginManager/            # 插件加载/卸载/调度管理器
│  ├─ ConfigParser/             # JSON全局配置解析器
│  ├─ EventBus/                 # 优化版全局事件总线（定向订阅+异步+节流）
│  ├─ Cell/                     # 细胞核心（八大生物铁律+AI+状态机）
│  ├─ EcologyCore/              # 生态核心计算逻辑（混合模块）
│  ├─ Lineage/                  # 基因&基础谱系&存档（混合模块）
│  ├─ Interaction/              # 交互底层核心（混合模块）
│  ├─ Render/                   # 底层渲染管线
│  ├─ UI/                       # 基础UI控件（静态）
│  └─ MainWindow/               # 主窗口 + 60FPS全局主循环
├─ Log/                         # 自动生成日志目录（按天分割日志文件）
├─ Bin/                         # 主EXE编译输出目录
└─ Doc/                         # 项目文档（略）