
| Element | Recommended Style | Example |
| --- | --- | --- |
| Classes and structs | `PascalCase` | `Player`, `EnemySpawner` |
| Enum types | `PascalCase` | `GameState`, `Direction` |
| Enum values | `PascalCase` | `GameState::Playing` |
| Functions | `camelCase` | `updatePlayer()` |
| Local variables | `camelCase` | `playerHealth` |
| Function parameters | `camelCase` | `deltaTimeSeconds` |
| Private members | `camelCase_` | `healthPoints_` |
| Constants | `kPascalCase` | `kTileSizePixels` |
| Macros | `SCREAMING_SNAKE_CASE` | `GAME_ASSERT` |
| Namespaces | Lowercase | `game`, `rendering` |
| Boolean variables | Start with `is`, `has`, `can`, or `should` | `isAlive`, `hasKey` |



| 类型 | 用途 |
|---|---|
| `feat` | 新增功能 |
| `fix` | 修复问题 |
| `refactor` | 重构，不改变外部行为 |
| `docs` | 修改文档 |
| `test` | 添加或修改测试 |
| `chore` | 日常维护、工具配置 |
| `build` / `ci` | 构建或持续集成相关修改 |