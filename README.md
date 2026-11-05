# DMR
Download more RAM to your PS Vita.

This plugin allows memory budget grow for game applications on PS Vita.

Maximum MAIN budget is limited at 381 MiB.
# Usage
1. Add plugin in taihen `config.txt` under `*KERNEL`
2. Use `int sceAppMgrGrowMemory(SceSize size, int killMiniApps)` to grow the MAIN memory budget of your application.
