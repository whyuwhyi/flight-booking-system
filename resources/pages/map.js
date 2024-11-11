var map = L.map('map').setView([39.9, 116.4], 5);

// 添加地图瓦片层
L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    maxZoom: 19,
    attribution: '&copy; OpenStreetMap contributors'
}).addTo(map);

var userOrders = []; // 存储用户订单信息
var routes = []; // 存储航线信息

new QWebChannel(qt.webChannelTransport, function(channel) {
    window.qt_map = channel.objects.qt_map;

    if (window.qt_map) {
        window.qt_map.sendRoutesData.connect(function(routesData) {
            if (routesData) {
                loadRoutesData(routesData);
            } else {
                console.error("No data received from backend");
            }
        });
        window.qt_map.sendUserOrderInfo.connect(function(orderInfo) {
            if (orderInfo) {
                parseUserOrders(orderInfo);
            } else {
                console.error("No order data received from backend");
            }
        });

    } else {
        console.error("Backend object or sendRoutesData signal not found");
    }
});

function parseUserOrders(orderData) {
    if (!orderData) {
        console.error("Invalid order data");
        return;
    }

    var lines = orderData.split('\n').map(function(line) {
        return line.trim();
    }).filter(function(line) {
        return line.length > 0;
    });

    userOrders = [];
    var i = 0;

    // 读取并跳过第一行的订单数
    var totalOrders = parseInt(lines[i++]);
    if (isNaN(totalOrders) || totalOrders <= 0) {
        console.error("Invalid total order count: " + totalOrders);
        return;
    }
    
    while (i < lines.length) {
        var order = {};
        if (i + 10 > lines.length) {
            console.error("Incomplete order data at line " + i);
            break;
        }

        order.orderNum = lines[i++]; // 订单号
        order.flightNumber = lines[i++];
        order.routeName = lines[i++];
        order.phoneNumber = lines[i++];
        order.passengerInfo = lines[i++];
        order.seatClassPrice = lines[i++];
        order.flightDate = lines[i++];
        order.seatNumber = lines[i++];
        order.mealType = lines[i++];
        order.orderStatus = lines[i++];

        // 只保留状态不为 1 或 2 的订单
        if (order.orderStatus !== '1' && order.orderStatus !== '3') {
            userOrders.push(order);
        }
    }
}


function normalizeLongitude360(lng) {
    if (lng < 0) {
        return lng + 360;
    }
    return lng;
}

function loadRoutesData(routesData) {
    if (!routesData) {
        console.error("Invalid route data");
        return;
    }

    var lines = routesData.split('\n').map(function(line) {
        return line.trim();
    }).filter(function(line) {
        return line.length > 0;
    });

    routes = [];
    var i = 0;

    var numRoutes = parseInt(lines[i++]);

    for (var routeIndex = 0; routeIndex < numRoutes; routeIndex++) {
        if (i + 4 >= lines.length) {
            console.error("Not enough data to parse route " + (routeIndex + 1));
            break;
        }

        var routeName = lines[i++];
        var startAirport = lines[i++];
        var endAirport = lines[i++];
        var length = parseFloat(lines[i++]);
        var pointCount = parseInt(lines[i++]);
        var points = [];

        for (var j = 0; j < pointCount; j++) {
            if (i >= lines.length) {
                console.error("Unexpected end of data while parsing coordinates for route " + routeName);
                break;
            }
            var coordsLine = lines[i++];
            var coords = coordsLine.split(' ').map(Number);
            if (coords.length !== 2 || isNaN(coords[0]) || isNaN(coords[1])) {
                console.error("Invalid coordinate data: " + coordsLine);
                continue;
            }
            points.push({ lat: coords[0], lng: normalizeLongitude360(coords[1]) });
        }

        routes.push({
            name: routeName,
            departureAirport: startAirport,
            arrivalAirport: endAirport,
            points: points,
            length: length
        });
    }

    // 初始绘制航线，不显示频率信息
    drawRoutes(routes, {});
}

function drawRoutes(routes, routeFrequency) {
    // 清除地图上的已有图层
    map.eachLayer(function(layer) {
        if (layer instanceof L.Curve || layer instanceof L.Marker) {
            map.removeLayer(layer);
        }
    });

    routes.forEach(function(route) {
        // 获取该航线的飞行次数
        var frequency = routeFrequency[route.name] || 0;

        // 根据频率调整航线样式
        var weight = 3; // 默认线宽
        var color = 'blue'; // 默认颜色

        if (frequency > 0) {
            // 增加线宽，改变颜色
            weight = 3 + frequency; // 线宽随频率增加
            color = 'red'; // 有飞行记录的航线显示为红色
        }

        // 现有的路径绘制代码
        var path = [];
        path.push('M', [route.points[0].lat, route.points[0].lng]);

        for (var i = 0; i < route.points.length - 1; i++) {
            var p0 = route.points[i - 1] || route.points[i];
            var p1 = route.points[i];
            var p2 = route.points[i + 1];
            var p3 = route.points[i + 2] || p2;

            var controlLat1 = p1.lat + (p2.lat - p0.lat) / 6;
            var controlLng1 = p1.lng + (p2.lng - p0.lng) / 6;
            var controlLat2 = p2.lat - (p3.lat - p1.lat) / 6;
            var controlLng2 = p2.lng - (p3.lng - p1.lng) / 6;

            path.push('C', [controlLat1, controlLng1], [controlLat2, controlLng2], [p2.lat, p2.lng]);
        }

        var curvePath = L.curve(path, {
            color: color,
            weight: weight,
            opacity: 0.7
        }).addTo(map);

        // 为航线添加点击事件，显示飞行次数
        curvePath.on('click', function(e) {
            var popupContent = '航线: ' + route.name + '<br>长度: ' + route.length.toFixed(2) + ' km';
            if (frequency > 0) {
                popupContent += '<br>您在所选时间段内飞行了 ' + frequency + ' 次此航线';
            } else {
                popupContent += '<br>您在所选时间段内未飞行此航线';
            }
            L.popup()
                .setLatLng(e.latlng)
                .setContent(popupContent)
                .openOn(map);
        });

        // 绘制起点和终点标记
        L.marker([route.points[0].lat, route.points[0].lng]).addTo(map)
            .bindPopup('起点机场: ' + route.departureAirport);

        L.marker([route.points[route.points.length - 1].lat, route.points[route.points.length - 1].lng]).addTo(map)
            .bindPopup('终点机场: ' + route.arrivalAirport);
    });
}

function updateRoutesDisplay(startDateStr, endDateStr) {
    var startDate = new Date(startDateStr);
    var endDate = new Date(endDateStr);

    // 统计每条航线的飞行次数
    var routeFrequency = {};

    userOrders.forEach(function(order) {
        var flightDate = new Date(order.flightDate);
        if (flightDate >= startDate && flightDate <= endDate) {
            var routeName = order.routeName;
            if (!routeFrequency[routeName]) {
                routeFrequency[routeName] = 0;
            }
            routeFrequency[routeName]++;
        }
    });

    console.log("Route frequencies:", routeFrequency);

    // 根据飞行次数更新航线样式
    drawRoutes(routes, routeFrequency);
}

// 添加按钮的点击事件监听
document.getElementById('updateButton').addEventListener('click', function() {
    var startDate = document.getElementById('startDate').value;
    var endDate = document.getElementById('endDate').value;

    if (!startDate || !endDate) {
        alert("请选择起始日期和结束日期");
        return;
    }

    updateRoutesDisplay(startDate, endDate);
});
