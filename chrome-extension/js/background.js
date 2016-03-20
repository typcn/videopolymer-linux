var port = 18069;

chrome.browserAction.onClicked.addListener(function(tab) {
   //chrome.tabs.executeScript(null, {file: "testScript.js"});
   sendToClient('youget',{url:tab.url.replace('acfun.tudou.com','www.acfun.tv'), title:tab.title});
});

function sendToClient(type,data) {
  if(port > 18079){
    port = 18069;
  }
  var request = new XMLHttpRequest();
  request.open('POST', 'http://localhost:' + port + '/vplay/' + type, true);
  request.onerror = function() {
    port++;
    sendToClient(type,data);
  }
  request.send(JSON.stringify(data));
}
