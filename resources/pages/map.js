var map = L.map('map').setView([39.9, 116.4], 5);

L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    maxZoom: 19,
    attribution: '&copy; OpenStreetMap contributors'
}).addTo(map);

var userOrders = [];
var routes = [];

new QWebChannel(qt.webChannelTransport, function(channel) {
    window.qt_map = channel.objects.qt_map;

    if (window.qt_map) {
        window.qt_map.sendRoutesData.connect(function(routesData) {
            if (routesData) {
                loadRoutesData(routesData);
            }
        });
        window.qt_map.sendUserOrderInfo.connect(function(orderInfo) {
            if (orderInfo) {
                parseUserOrders(orderInfo);
            }
        });
    }
});

function parseUserOrders(orderData) {
    if (!orderData) return;

    var lines = orderData.split('\n').map(function(line) {
        return line.trim();
    }).filter(function(line) {
        return line.length > 0;
    });

    userOrders = [];
    var i = 0;
    var totalOrders = parseInt(lines[i++]);
    if (isNaN(totalOrders) || totalOrders <= 0) return;

    while (i < lines.length) {
        var order = {};
        if (i + 10 > lines.length) break;

        order.orderNum = lines[i++];
        order.flightNumber = lines[i++];
        order.routeName = lines[i++];
        order.phoneNumber = lines[i++];
        order.passengerInfo = lines[i++];
        order.seatClassPrice = lines[i++];
        order.flightDate = lines[i++];
        order.seatNumber = lines[i++];
        order.mealType = lines[i++];
        order.orderStatus = lines[i++];

        if (order.orderStatus !== '1' && order.orderStatus !== '3') {
            userOrders.push(order);
        }
    }
}

function normalizeLongitude360(lng) {
    return lng < 0 ? lng + 360 : lng;
}

function loadRoutesData(routesData) {
    if (!routesData) return;

    var lines = routesData.split('\n').map(function(line) {
        return line.trim();
    }).filter(function(line) {
        return line.length > 0;
    });

    routes = [];
    var i = 0;
    var numRoutes = parseInt(lines[i++]);

    for (var routeIndex = 0; routeIndex < numRoutes; routeIndex++) {
        if (i + 4 >= lines.length) break;

        var routeName = lines[i++];
        var startAirport = lines[i++];
        var endAirport = lines[i++];
        var length = parseFloat(lines[i++]);
        var pointCount = parseInt(lines[i++]);
        var points = [];

        for (var j = 0; j < pointCount; j++) {
            if (i >= lines.length) break;
            var coordsLine = lines[i++];
            var coords = coordsLine.split(' ').map(Number);
            if (coords.length !== 2 || isNaN(coords[0]) || isNaN(coords[1])) continue;
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

    drawRoutes(routes, {}, {});
}

function drawRoutes(routes, routeFrequency, routeFlightDetails) {
    map.eachLayer(function(layer) {
        if (layer instanceof L.Curve || layer instanceof L.Marker) {
            map.removeLayer(layer);
        }
    });

    routes.forEach(function(route) {
        var frequency = routeFrequency[route.name] || 0;
        var flightDetails = routeFlightDetails[route.name] || {};

        var weight = 3;
        var color = 'blue';

        if (frequency > 0) {
            weight = 3 + frequency;
            color = 'red';
        }

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

        curvePath.on('click', function(e) {
            var popupContent = '航线: ' + route.name + '<br>长度: ' + route.length.toFixed(2) + ' km';
            if (frequency > 0) {
                popupContent += '<br>您在所选时间段内飞行了 ' + frequency + ' 次此航线';
                popupContent += '<br><br>航班详情：';
                for (var flightNumber in flightDetails) {
                    popupContent += '<br>航班号: ' + flightNumber + ' - ' + flightDetails[flightNumber] + ' 次';
                }
            } else {
                popupContent += '<br>您在所选时间段内未飞行此航线';
            }
            L.popup()
                .setLatLng(e.latlng)
                .setContent(popupContent)
                .openOn(map);
        });

        L.marker([route.points[0].lat, route.points[0].lng]).addTo(map)
            .bindPopup('起点机场: ' + route.departureAirport);

        L.marker([route.points[route.points.length - 1].lat, route.points[route.points.length - 1].lng]).addTo(map)
            .bindPopup('终点机场: ' + route.arrivalAirport);
    });
}

function updateRoutesDisplay(startDateStr, endDateStr) {
    var startDate = new Date(startDateStr);
    var endDate = new Date(endDateStr);

    var routeFrequency = {};
    var routeFlightDetails = {};

    userOrders.forEach(function(order) {
        var flightDate = new Date(order.flightDate);
        if (flightDate >= startDate && flightDate <= endDate) {
            var routeName = order.routeName;
            var flightNumber = order.flightNumber;

            if (!routeFrequency[routeName]) {
                routeFrequency[routeName] = 0;
            }
            routeFrequency[routeName]++;

            if (!routeFlightDetails[routeName]) {
                routeFlightDetails[routeName] = {};
            }
            if (!routeFlightDetails[routeName][flightNumber]) {
                routeFlightDetails[routeName][flightNumber] = 0;
            }
            routeFlightDetails[routeName][flightNumber]++;
        }
    });

    drawRoutes(routes, routeFrequency, routeFlightDetails);
}

document.getElementById('updateButton').addEventListener('click', function() {
    var startDate = document.getElementById('startDate').value;
    var endDate = document.getElementById('endDate').value;

    if (!startDate || !endDate) {
        alert("请选择起始日期和结束日期");
        return;
    }

    updateRoutesDisplay(startDate, endDate);
});
