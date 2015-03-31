var windowInfo = require('./build/Release/window-info');

setInterval(function () {
    windowInfo(function (windowTitle, processPath) {
        console.log("title: ", windowTitle);
        console.log("path: ", processPath);
    })
}, 1000);