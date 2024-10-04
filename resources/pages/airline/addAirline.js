var map = L.map('map').setView([39.9, 116.4], 5);

new QWebChannel(qt.webChannelTransport, function(channel) {
    window.qt_addAirline = channel.objects.qt_addAirline;
});

L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    maxZoom: 19,
}).addTo(map);

var suggestionsElement = document.getElementById('suggestions');
var airport1Input = document.getElementById('airport1');
var airport2Input = document.getElementById('airport2');
var confirmRouteButton = document.getElementById('confirmRouteButton');
var citySearchInput = document.getElementById('citySearch');
var routePoints = [];
var markers = [];
var polylines = [];
var routeLengthInKm = 0;

citySearchInput.addEventListener('input', function() {
    var cityName = this.value;
    if (cityName) {
        fetch(`https://nominatim.openstreetmap.org/search?q=${cityName}&format=json`)
            .then(response => response.json())
            .then(data => {
                suggestionsElement.innerHTML = '';
                if (data.length) {
                    suggestionsElement.style.display = 'block';
                    data.forEach(item => {
                        var suggestionItem = document.createElement('div');
                        suggestionItem.className = 'suggestion-item';
                        suggestionItem.textContent = item.display_name;
                        suggestionItem.addEventListener('click', function() {
                            addCityToRoute(item);
                        });
                        suggestionsElement.appendChild(suggestionItem);
                    });
                } else {
                    suggestionsElement.style.display = 'none';
                }
            });
    } else {
        suggestionsElement.style.display = 'none';
    }
});

function addCityToRoute(item) {
    var lat = parseFloat(item.lat);
    var lng = parseFloat(item.lon);
    var cityName = item.display_name.split(',')[0];

    var marker = L.marker([lat, lng]).addTo(map).bindPopup(cityName);
    marker.on('click', function() {
        removeCityFromRoute(lat, lng);
    });

    markers.push(marker);

    routePoints.splice(routePoints.length - 1, 0, { lat: lat, lng: lng });

    drawRoute();

    suggestionsElement.style.display = 'none';
}

function removeCityFromRoute(lat, lng) {
    var indexToRemove = routePoints.findIndex(function(point) {
        return point.lat === lat && point.lng === lng;
    });

    if (indexToRemove > 0 && indexToRemove < routePoints.length - 1) {
        routePoints.splice(indexToRemove, 1);

        markers = markers.filter(function(marker) {
            if (marker.getLatLng().lat === lat && marker.getLatLng().lng === lng) {
                map.removeLayer(marker);
                return false;
            }
            return true;
        });

        drawRoute();
    } else {
        alert("不能删除机场标记！");
    }
}

function drawRoute() {
    polylines.forEach(function(polyline) {
        map.removeLayer(polyline);
    });
    polylines = [];

    if (routePoints.length > 1) {
        var path = [];

        path.push('M', [routePoints[0].lat, routePoints[0].lng]);

        var bezierCurves = [];

        for (var i = 0; i < routePoints.length - 1; i++) {
            var p0 = routePoints[i - 1] || routePoints[i];
            var p1 = routePoints[i];
            var p2 = routePoints[i + 1];
            var p3 = routePoints[i + 2] || p2;

            var controlLat1 = p1.lat + (p2.lat - p0.lat) / 6;
            var controlLng1 = p1.lng + (p2.lng - p0.lng) / 6;

            var controlLat2 = p2.lat - (p3.lat - p1.lat) / 6;
            var controlLng2 = p2.lng - (p3.lng - p1.lng) / 6;

            path.push('C', [controlLat1, controlLng1], [controlLat2, controlLng2], [p2.lat, p2.lng]);

            bezierCurves.push({
                p1: L.latLng(p1.lat, p1.lng),
                cp1: L.latLng(controlLat1, controlLng1),
                cp2: L.latLng(controlLat2, controlLng2),
                p2: L.latLng(p2.lat, p2.lng)
            });
        }

        var curvePath = L.curve(path, { color: 'blue', weight: 2 }).addTo(map);
        polylines.push(curvePath);

        calculateRouteLength(bezierCurves);
    } else {
        routeLengthInKm = 0;
    }
}

function calculateRouteLength(bezierCurves) {
    var totalLength = 0;
    var samplesPerCurve = 50;

    bezierCurves.forEach(function(curve) {
        var previousPoint = curve.p1;
        for (var t = 1; t <= samplesPerCurve; t++) {
            var ratio = t / samplesPerCurve;
            var point = getCubicBezierPoint(curve.p1, curve.cp1, curve.cp2, curve.p2, ratio);
            totalLength += previousPoint.distanceTo(point);
            previousPoint = point;
        }
    });

    routeLengthInKm = totalLength / 1000;
}

function getCubicBezierPoint(p0, p1, p2, p3, t) {
    var lat = cubicN(p0.lat, p1.lat, p2.lat, p3.lat, t);
    var lng = cubicN(p0.lng, p1.lng, p2.lng, p3.lng, t);
    return L.latLng(lat, lng);
}

function cubicN(a, b, c, d, t) {
    var t2 = t * t;
    var t3 = t2 * t;
    return a * (-t3 + 3 * t2 - 3 * t + 1) +
           b * (3 * t3 - 6 * t2 + 3 * t) +
           c * (-3 * t3 + 3 * t2) +
           d * t3;
}

confirmRouteButton.addEventListener('click', function() {
    if (window.qt_addAirline) {
        window.qt_addAirline.receiveRouteData(routePoints, routeLengthInKm);
    }
});

document.addEventListener('click', function(event) {
    if (!suggestionsElement.contains(event.target) && event.target !== citySearchInput) {
        suggestionsElement.style.display = 'none';
    }
});

citySearchInput.addEventListener('focus', function() {
    if (suggestionsElement.childNodes.length > 0) {
        suggestionsElement.style.display = 'block';
    }
});

window.onload = function() {
    new QWebChannel(qt.webChannelTransport, function(channel) {
        window.qt_addAirline = channel.objects.qt_addAirline;

        if (window.qt_addAirline) {
            window.qt_addAirline.airlineDataReceived.connect(function(airport1, airport2, lat1, lng1, lat2, lng2) {
                airport1Input.value = airport1;
                airport2Input.value = airport2;

                var airport1Marker = L.marker([lat1, lng1]).addTo(map).bindPopup(airport1);
                var airport2Marker = L.marker([lat2, lng2]).addTo(map).bindPopup(airport2);

                routePoints = [
                    { lat: lat1, lng: lng1 },
                    { lat: lat2, lng: lng2 }
                ];

                drawRoute();
            });

            window.qt_addAirline.requestAirlineData();
        }
    });
};
