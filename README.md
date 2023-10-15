# <div align="center">cs2-DMA-radar</div>

<div align="center">
    <img src="https://img.shields.io/badge/License-MIT-important?style=flat-square" />
    <img src="https://img.shields.io/badge/%E2%80%8E-C++-00599C?style=flat-square&logoWidth=20&logo=image/svg+xml;base64,PHN2ZyByb2xlPSJpbWciIHZpZXdCb3g9IjAgMCAyNCAyNCIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj48dGl0bGU+QysrIGljb248L3RpdGxlPjxwYXRoIGZpbGw9IndoaXRlIiBkPSJNMjIuMzkzIDZjLS4xNjctLjI5LS4zOTgtLjU0My0uNjUyLS42OUwxMi45MjUuMjJjLS41MDgtLjI5My0xLjMzOS0uMjkzLTEuODQ3IDBMMi4yNiA1LjMxYy0uNTA4LjI5My0uOTIzIDEuMDEzLS45MjMgMS42djEwLjE4YzAgLjI5NC4xMDQuNjIuMjcxLjkxLjE2Ny4yOS4zOTguNTQzLjY1Mi42ODlsOC44MTYgNS4wOTFjLjUwOC4yOTMgMS4zMzkuMjkzIDEuODQ3IDBsOC44MTYtNS4wOTFjLjI1NC0uMTQ2LjQ4NS0uMzk5LjY1Mi0uNjg5cy4yNzEtLjYxNi4yNzEtLjkxVjYuOTFjLjAwMi0uMjk0LS4xMDItLjYyLS4yNjktLjkxek0xMiAxOS4xMDljLTMuOTIgMC03LjEwOS0zLjE4OS03LjEwOS03LjEwOVM4LjA4IDQuODkxIDEyIDQuODkxYTcuMTMzIDcuMTMzIDAgMCAxIDYuMTU2IDMuNTUybC0zLjA3NiAxLjc4MUEzLjU2NyAzLjU2NyAwIDAgMCAxMiA4LjQ0NWMtMS45NiAwLTMuNTU0IDEuNTk1LTMuNTU0IDMuNTU1UzEwLjA0IDE1LjU1NSAxMiAxNS41NTVhMy41NyAzLjU3IDAgMCAwIDMuMDgtMS43NzhsMy4wNzcgMS43OEE3LjEzNSA3LjEzNSAwIDAgMSAxMiAxOS4xMDl6bTcuMTA5LTYuNzE0aC0uNzl2Ljc5aC0uNzl2LS43OWgtLjc5di0uNzloLjc5di0uNzloLjc5di43OWguNzl2Ljc5em0yLjk2MiAwaC0uNzl2Ljc5aC0uNzl2LS43OWgtLjc4OXYtLjc5aC43ODl2LS43OWguNzl2Ljc5aC43OXYuNzl6Ii8+PC9zdmc+" />
    <img src="https://img.shields.io/badge/%E2%80%8E-NodeJS-339933?style=flat-square&logoWidth=20&logo=node.js" />
    <br/><br/>
    Scuffed external radar built with C++ and NodeJS. <br>
    Download can be found here: <a href="https://www.unknowncheats.me/forum/cs-go-releases/433334-external-radar-nodejs.html#post3020185">unknowncheats</a>
</div>

___

## Preview

![](preview.gif)


## Requirements
- node LTS version (nodejs.org)
## Build

Radar:
- Release x86

Server:
- Install pkg globally

```
npm install pkg -g
// or
yarn global add pkg
```
- Build the Server with the following command
```
pkg server/server.js --config package.json --compress
```
---
Folder Structure:
```
📁 root/
├─ csgo-external-radar.exe
├─ radar.exe
├─ 📁 dependencies/
│  ├─ texconv.exe
```
## Usage

- Navigate to /csgo/resource/overviews then copy contents to /app/resource/overviews on second pc
- Start the radar
- Start CS2 and wait until you are in the main menu (or until the offsets are visible in the console)
- Start the server (`node server/server.js`)
- Visit localhost:3000 in your favorite browser

## Features

- Enemy
    - Active
    - Health
    - Name
    - Weapon

- Team
    - Active
    - Health
    - Name
    - Weapon
- Local
    - Active


## Credits
Mortale <br>
[unknowncheats](https://www.unknowncheats.me/forum/members/692168.html) | [Github](https://github.com/M0rtale) <br><br>
https://github.com/frk1/hazedumper <br>
https://github.com/microsoft/DirectXTex/tree/master/Texconv <br>
https://github.com/node-steam/vdf <br>
https://github.com/nlohmann/json
