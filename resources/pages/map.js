var map = L.map('map').setView([39.9, 116.4], 5);

L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    maxZoom: 19,
    attribution: '&copy; OpenStreetMap contributors'
}).addTo(map);

var userOrders = [];
var routes = [];

var timeSlotStats = { MORNING: 0, AFTERNOON: 0, EVENING: 0, NIGHT: 0 };
var airlineStats = {};
var airplaneModelStats = {};

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
                generateUserProfile();
                var startDate = document.getElementById('startDate').value;
                var endDate = document.getElementById('endDate').value;
            
                if (!startDate || !endDate) {
                    alert("请选择起始日期和结束日期");
                    return;
                }
            
                updateRoutesDisplay(startDate, endDate);
            }
        });
        qt_map.requestRoutesData();
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
    timeSlotStats = { MORNING: 0, AFTERNOON: 0, EVENING: 0, NIGHT: 0 };
    airlineStats = {};
    airplaneModelStats = {};

    var i = 0;
    var totalOrders = parseInt(lines[i++]);
    if (isNaN(totalOrders) || totalOrders <= 0) return;

    for (var orderIndex = 0; orderIndex < totalOrders; orderIndex++) {
        if (i >= lines.length) break;

        var order = {};
        order.orderNum = lines[i++];
        order.phoneNumber = lines[i++];
        order.orderStatus = parseInt(lines[i++]); // Adjusted: removed passengerInfo
        var numSegments = parseInt(lines[i++]);

        for (var j = 0; j < numSegments; j++) {
            if (i + 9 > lines.length) break; // Adjusted line count per segment

            var segment = {};
            segment.flightNumber = lines[i++];
            segment.airRoute = lines[i++];
            segment.airplaneModel = lines[i++];
            segment.airline = lines[i++];
            segment.departureTimeSlot = parseInt(lines[i++]);
            segment.cabinType = parseInt(lines[i++]);
            segment.price = parseFloat(lines[i++]);
            segment.flightDate = lines[i++];

            // Read Passenger Data
            if (i >= lines.length) break;
            var passengerLine = lines[i++];
            var passengerTokens = passengerLine.split(' ');
            if (passengerTokens.length < 4) {
                console.error('Invalid passenger info format at line ' + (i - 1));
                continue;
            }

            var passenger = {};
            passenger.name = passengerTokens[0];
            passenger.idNumber = passengerTokens[1];
            passenger.seatNumber = passengerTokens[2];
            passenger.mealType = parseInt(passengerTokens[3]);

            if (order.orderStatus === 0 || order.orderStatus === 2) {
                // Update statistics
                if (segment.departureTimeSlot === 0) timeSlotStats.MORNING++;
                else if (segment.departureTimeSlot === 1) timeSlotStats.AFTERNOON++;
                else if (segment.departureTimeSlot === 2) timeSlotStats.EVENING++;
                else if (segment.departureTimeSlot === 3) timeSlotStats.NIGHT++;

                if (!airlineStats[segment.airline]) {
                    airlineStats[segment.airline] = 0;
                }
                airlineStats[segment.airline]++;

                if (!airplaneModelStats[segment.airplaneModel]) {
                    airplaneModelStats[segment.airplaneModel] = 0;
                }
                airplaneModelStats[segment.airplaneModel]++;

                userOrders.push({
                    orderNum: order.orderNum,
                    flightNumber: segment.flightNumber,
                    airRoute: segment.airRoute,
                    airplaneModel: segment.airplaneModel,
                    airline: segment.airline,
                    departureTimeSlot: segment.departureTimeSlot,
                    cabinType: segment.cabinType,
                    price: segment.price,
                    flightDate: segment.flightDate,
                    seatNumber: passenger.seatNumber,
                    mealType: passenger.mealType,
                    passengerName: passenger.name,
                    passengerIdNumber: passenger.idNumber,
                    phoneNumber: order.phoneNumber,
                    orderStatus: order.orderStatus
                });
            }
        }
    }
}


function generateUserProfile() {
    if (userOrders.length === 0) {
        alert("没有出行记录");
        return;
    }

    var mostPreferredTimeSlot = Object.keys(timeSlotStats).reduce((a, b) => timeSlotStats[a] > timeSlotStats[b] ? a : b);
    var mostPreferredAirline = Object.keys(airlineStats).reduce((a, b) => airlineStats[a] > airlineStats[b] ? a : b, "无");
    var mostPreferredAirplaneModel = Object.keys(airplaneModelStats).reduce((a, b) => airplaneModelStats[a] > airplaneModelStats[b] ? a : b, "无");

    var profileHtml = `
        <h2 style="color: #333; border-bottom: 2px solid #007bff; padding-bottom: 5px;">您的出行偏好分析</h2>
        <div style="margin-bottom: 10px;">
            <span style="font-weight: bold; color: #007bff;">最偏好出行时间段：</span>
            <span>${translateTimeSlot(mostPreferredTimeSlot)}，共出行 ${timeSlotStats[mostPreferredTimeSlot]} 次。</span>
        </div>
        <div style="margin-bottom: 10px;">
            <span style="font-weight: bold; color: #007bff;">最常乘坐的航空公司：</span>
            <span>${mostPreferredAirline}，共乘坐 ${airlineStats[mostPreferredAirline]} 次。</span>
        </div>
        <div style="margin-bottom: 10px;">
            <span style="font-weight: bold; color: #007bff;">最常乘坐的机型：</span>
            <span>${mostPreferredAirplaneModel}，共乘坐 ${airplaneModelStats[mostPreferredAirplaneModel]} 次。</span>
        </div>
    `;

    var profileDiv = document.getElementById('userProfile');
    if (!profileDiv) {
        profileDiv = document.createElement('div');
        profileDiv.id = 'userProfile';
        profileDiv.style.padding = '15px';
        profileDiv.style.border = '1px solid #ccc';
        profileDiv.style.borderRadius = '8px';
        profileDiv.style.marginTop = '15px';
        profileDiv.style.backgroundColor = '#f5faff';
        profileDiv.style.fontSize = '16px';
        profileDiv.style.boxShadow = '0 2px 4px rgba(0, 0, 0, 0.1)';
        document.body.appendChild(profileDiv);
    }
    profileDiv.innerHTML = profileHtml;
}


function translateTimeSlot(slot) {
    switch (slot) {
        case 'MORNING': return "早晨";
        case 'AFTERNOON': return "下午";
        case 'EVENING': return "晚上";
        case 'NIGHT': return "夜间";
        default: return "未知时段";
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
            var routeName = order.airRoute;
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
    qt_map.requestUserOrderInfo()
});
