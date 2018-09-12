function postMessage (data, extra) {
  clients.matchAll().then(function (clients) {
    clients.forEach(function (client) {
      client.postMessage(data, extra)
    })
  })
}

function fetchAndCache (request, cache) {
  return fetch((request instanceof Request) ? request.clone() : request).then(function (response) {
    if (response && response.status === 200 && response.type === 'basic') {
      caches.open(cache || cacheName).then(function (cache) {
        cache.put(request, response)
      })
    }
    return response.clone()
  })
}

function fromCacheNetLater (request) {
  return caches.match(request).then(function (response) {
    var netRes = fetchAndCache(request)
    if (response)
      return response
    return netRes
  })
}
function fromCache (request) {
  return caches.match(request)
}
function fromNetCacheLater (request) {
  return new Promise(function (resolve, reject) {
    fetchAndCache(request).then(function (response) {
      if (response && response.status === 200 && response.type === 'basic') {
        resolve(response)
        return
      }
      resolve(fromCache(request))
    }).catch(function (e) {
      resolve(fromCache(request))
    })
  })
}
//

var cacheName = 'tkb-v2.1'
var cacheFirstNetLater = [
  '',
  'index.html',
  'manifest.json',
  'Calendar_files/img/1f605.png',
  'Calendar_files/img/ic.png',
  'Calendar_files/icon.png',
  'data.js'
]
var ignoreRegex = [
  'sw\\.js'
]
var defaultCache = fromCacheNetLater

self.addEventListener('install', function (e) {
  console.log('[ServiceWorker] Install')
  for (var i = 0; i < cacheFirstNetLater.length; i++) {
    console.log('FetchAndCache:', cacheFirstNetLater[i])
    fetchAndCache(cacheFirstNetLater[i])
  }
  return self.skipWaiting()
})

self.addEventListener('activate', function (e) {
  console.log('[ServiceWorker] Activate')
  e.waitUntil(caches.keys().then(function (keyList) {
    return Promise.all(keyList.map(function (key) {
      if (key !== cacheName) {
        console.log('[ServiceWorker] Removing old cache:', key)
        return caches.delete(key)
      }
    }))
  }))
  return self.clients.claim()
})

self.addEventListener('fetch', function (e) {
  var request = e.request
  var url = request.url
  console.log('Fetch event for:', url)
  for (var i = 0; i < ignoreRegex.length; i++) {
    var regex = new RegExp(ignoreRegex[i])
    if (regex.test(url))
      return fetch(request.clone())
  }
  e.respondWith(defaultCache(request))
  if (/data\.js/.test(url))
    postMessage('dataOK')
})
