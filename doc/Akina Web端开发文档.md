# Akina Web端开发文档

## 简介

软件名称：Akina订票系统

开发环境：Windows 10、Ubuntu18.04

测试环境：Windows 10、Ubuntu18.04

开发语言：HTML+CSS+JavaScript+Python3.6

使用框架：Django、Jinja2、Bootstrap4、Jquery、bootstrap-table、bootstrap-select、MathJax

开发人员：张宇恒、杨宗翰

## 文件功能说明

- `/static` 静态文件，包括css，js等
- `/templates` 模板
- `/view.py` 主要功能实现
- `/inputchecker.py` 输入检查相关
- `/uclient.py` 前后端交互
- `/urls.py` 路由



## 模块功能说明

`/` 首页

`/login` 登录以及注册页面

`/logout` 登出

`/train`  车票查询（包括中转，购票）

`/ticket` 已购车票查询（包括退票）

`/manage` 车次管理（包括查询，售卖）

`/person` 用户信息查看（可进行更改操作）

`/addtrain` 添加车次

`/clean` 删库（需要以管理员登录）

`/aboutus` 关于我们

### 权限验证

session保存用户信息，在前端的后端进行。

### 页面

使用bootstrap4框架，辅以bootstrap-select、bootstrap-table插件进行优化。我们对css进行了更改，使其拥有透明的炫酷外观。

我们尽量使用单页页面，精简url。

主体功能居中显示，使其更加突出。

### 通信

前端和后端使用同步 socket 进行 ipv4 通信。前后端的充分解构，因此可以架设在不同服务器上。

### Bouns

传输过程使用 json 来实现了 token 验证：由于本次服务器实际架设时仅采用了一台服务器，我们可以认为服务器内网环境是安全的，因此采用了单一 token 进行验证；当前后端需要分离架设时，可以很方便地修改为带有效期的 token 以适应安全需要。

前端的后端与后端的前端均启用了断线自动重连机制：配合 daemon 程序可以保证产生崩溃时可以自动重启崩溃的模块。

前端对密码进行了不可逆加密 (SHA256) 以充分保护用户隐私。

实现了一个通用验证码：在发行的版本里采用了 MathJax 插件，使用简单的定积分作为验证码主体（考虑到用户为贵班学生）；可以简单地修改 `captcha.py` 里的内容来适应不同的用户群体。验证码内容存储在 Session 里，接口依赖于 python3 的动态类型，可以进行非常任意的修改，可以很方便地修改为任意类型的验证码（带干扰的图案 / 数学公式 / 自然语言），甚至可以调用 reCaptcha 等更加有效的验证码。

## 部署

在Ubuntu18.04环境下直接运行`run.sh` 

