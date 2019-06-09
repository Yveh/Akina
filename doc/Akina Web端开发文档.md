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

## 部署

在Ubuntu18.04环境下直接运行`run.sh` 

