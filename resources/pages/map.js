// 初始化地图
var map = L.map('map').setView([39.9, 116.4], 5);

L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    maxZoom: 19,
    attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
}).addTo(map);

window.onload = function() {
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
            window.qt_map.requestRoutesData();
        } else {
            console.error("Backend object or sendRoutesData signal not found");
        }
    });
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

    console.error(lines);
    var routes = [];
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
            points.push({ lat: coords[0], lng: coords[1] });
        }

        routes.push({
            name: routeName,
            departureAirport: startAirport,
            arrivalAirport: endAirport,
            points: points,
            length: length
        });
    }
    drawRoutes(routes);
}

function drawRoutes(routes) {
    routes.forEach(function(route) {
        if (route.points.length < 2) {
            console.error("Route has insufficient points to draw:", route.name);
            return;
        }

        var path = [];
        path.push('M', [route.points[0].lat, route.points[0].lng]);

        for (var i = 0; i < route.points.length - 1; i++) {
            var p0 = route.points[i - 1] || route.points[i];
            var p1 = route.points[i];
            var p2 = route.points[i + 1];
            var p3 = route.points[i + 2] || p2;

            if (!p1 || !p2 || !p1.lat || !p1.lng || !p2.lat || !p2.lng) {
                console.error("Invalid points found in route:", route.name);
                continue;
            }

            var controlLat1 = p1.lat + (p2.lat - p0.lat) / 6;
            var controlLng1 = p1.lng + (p2.lng - p0.lng) / 6;
            var controlLat2 = p2.lat - (p3.lat - p1.lat) / 6;
            var controlLng2 = p2.lng - (p3.lng - p1.lng) / 6;

            path.push('C', [controlLat1, controlLng1], [controlLat2, controlLng2], [p2.lat, p2.lng]);
        }

        var curvePath = L.curve(path, {
            color: 'blue',
            weight: 3,
            opacity: 0.7
        }).addTo(map);

        curvePath.on('click', function(e) {
            var popupContent = '航线: ' + route.name + '<br>长度: ' + route.length.toFixed(2) + ' km';
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
