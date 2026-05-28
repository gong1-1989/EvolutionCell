#include "GameGlobal.h"

/**
 * @brief 重构说明
 * 原代码大量重复 cfg["xxx"]["xxx"] 解析，现统一调用 ConfigReader 通用方法
 * 所有接口仅做「取值转发」，新增/修改配置仅需改 json + 接口声明，无需改动实现
 */
using namespace GameGlobal;


// ===================== 躯体解构配置 =====================
int GameGlobal::getDecomposeRisk(DecomposeLevel lv)
{
    switch (lv)
    {
    case DecomposeLevel::DECOMPOSE_LIGHT: return 10;
    case DecomposeLevel::DECOMPOSE_DEEP:  return 25;
    case DecomposeLevel::DECOMPOSE_FULL:  return 45;
    default: return 0;
    }
}
