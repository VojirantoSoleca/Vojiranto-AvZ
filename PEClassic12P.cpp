#include <avz.h>

bool judge1() // 收尾波延迟炮判定
{
    for (auto&& zombie : aAliveZombieFilter) {
        if ((!zombie.IsDead()) && zombie.Abscissa() > 400) {
            return true;
        }
    }
    return false;
}

bool judge2() // 漏汽球补丁
{
    for (auto&& zombie : aAliveZombieFilter) {
        if (AZombieType(zombie.Type()) == AQQ_16 && zombie.Abscissa() < 500) {
            return true;
        }
    }
    return false;
}

void AScript()
{
    ASelectCards({
        AICE_SHROOM,   // 寒冰菇
        AM_ICE_SHROOM, // 模仿寒冰菇
        ACOFFEE_BEAN,  // 咖啡豆
        ADOOM_SHROOM,  // 毁灭菇
        ALILY_PAD,     // 荷叶
        ASQUASH,       // 倭瓜
        ACHERRY_BOMB,  // 樱桃炸弹
        ABLOVER,       // 三叶草
        APUMPKIN,      // 南瓜头
        APUFF_SHROOM,  // 小喷菇
    });

    AConnect(ATime(1, -599), [] {
        aPainter.Draw(AText("已载入炮列表", 0, 100), 100);
        aCobManager.AutoGetList();
    });

    /**
     * P6
     * 主体节奏
     */
    for (auto wave = 1; wave <= 20; wave++) {
        AConnect(ATime(wave, 341 - 373), [] {
            aPainter.Draw(AText("发射标准波炮", 0, 100), 100);
            aCobManager.Fire({{2, 9}, {5, 9}});

            if (judge2()) {
                ACard(ABLOVER, 1, 1);
                aPainter.Draw(AText("漏汽球处理", 0, 160), 100);
            }
        });
    }

    /**
     * wave 9 19 20的附加操作
     * 收尾判定炮
     * 根据判定情况不同，有射0，2，4门炮的可能
     * 每次判定射两门炮，条件为前场是否有活着的僵尸
     * 后场小鬼不影响四门炮的判定
     */
    for (auto wave : {9, 19, 20}) {
        AConnect(ATime(wave, 341 - 373 + 600), [] {
            if (judge1()) {
                aPainter.Draw(AText("发射第一轮收尾炮", 0, 100), 100);
                aCobManager.RecoverFire({{2, 9}, {5, 9}});
            }
        });
        AConnect(ATime(wave, 341 - 373 + 1100), [] {
            if (judge1()) {
                aPainter.Draw(AText("发射第二轮收尾炮", 0, 100), 100);
                aCobManager.RecoverFire({{2, 9}, {5, 9}});
            }
        });
    }

    /**
     * wave 10 的附加操作
     * 樱桃窝瓜消延迟
     * 解决僵尸出生点靠右的问题
     * 防止红眼过多且偏右，炮炸伤害不足导致下波不按时刷新
     * 不按时刷新可能导致偏右没被炸到的水路僵尸啃炮
     */
    AConnect(ATime(10, 341 - 100), [] {
        aPainter.Draw(AText("樱桃消延迟", 0, 120), 100);
        ACard(ACHERRY_BOMB, 2, 9);
        aPainter.Draw(AText("窝瓜消延迟", 0, 140), 100);
        ACard(ASQUASH, 5, 9);
    });

    /**
     * wave 20 的附加操作
     * 咆哮珊瑚(炮消)
     * 解决僵尸出生点靠右的问题
     */
    AConnect(ATime(20, 250 - 378), [] {
        aPainter.Draw(AText("炮消珊瑚", 0, 120), 100);
        aCobManager.Fire(4, 7.625);
    });
}
