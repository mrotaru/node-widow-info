var windowInfo = require('./build/Release/window-info');

setInterval(function () {
    windowInfo(function (windowTitle) {
        console.log(windowTitle);
    })
}, 1000);