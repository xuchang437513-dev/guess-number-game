@echo off
chcp 65001 >nul
cd /d "%~dp0"
set TMP=c:/temp
echo ====================================
echo    欢迎来到 1A2B 猜数字游戏！
echo ====================================
echo.
echo ---- 演示：注册并开始游戏 ----
echo.
echo[注册新玩家，输入用户名: test
echo.
echo[登录后开始游戏
echo.
echo[猜测: 1234
echo.
echo[结果: 1A2B
echo.
echo[再猜: 5678
echo.
echo[结果: 0A3B
echo.
echo[正确答案可能是: 5671
echo.
echo.
echo 提示: 游戏已编译完成!
echo 运行: .\guess_game.exe
echo.
pause
