# node-window-info

Native node.js module for current window info. Only works on Windows, and can
return the window title and executable path for the top window.

```
setInterval(function () {
    windowInfo(function (windowTitle, processPath) {
        console.log("title: ", windowTitle);
        console.log("path: ", processPath);
    })
}, 1000);
```
