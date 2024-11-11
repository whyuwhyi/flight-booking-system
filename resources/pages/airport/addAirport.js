var map = L.map('map').setView([39.9, 116.4], 5);

new QWebChannel(qt.webChannelTransport, function(channel) {
    window.qt_addAirport = channel.objects.qt_addAirport; // 注册 qt 对象
});

L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    maxZoom: 19,
}).addTo(map);

var suggestionsElement = document.getElementById('suggestions');
var modal = document.getElementById('modal');
var airportNameInput = document.getElementById('airportName');
var airportLatInput = document.getElementById('airportLat');
var airportLngInput = document.getElementById('airportLng');
var airportCityElement = document.getElementById('airportCity');
var airportCountryElement = document.getElementById('airportCountry');

document.getElementById('airportSearch').addEventListener('input', function() {
    var airportName = this.value;
    if (airportName) {
        fetch(`https://nominatim.openstreetmap.org/search?q=${airportName} airport&format=json`)
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
                            displayAirport(item);
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

function displayAirport(item) {
    var lat = item.lat;
    var lng = item.lon;

    map.eachLayer(function(layer) {
        if (layer instanceof L.Marker) {
            map.removeLayer(layer);
        }
    });

    var marker = L.marker([lat, lng]).addTo(map);
    marker.bindPopup(item.display_name).openPopup();
    map.setView([lat, lng], 10);
    suggestionsElement.style.display = 'none';

    var parts = item.display_name.split(',');
    airportNameInput.value = parts[0].trim();
    airportLatInput.value = lat;
    airportLngInput.value = lng;

    // 识别国家，一定是最后一个字符串
    var country = parts[parts.length - 1].trim();
    airportCountryElement.value = country;

    // 识别城市
    var city = '未知城市';
    for (var i = parts.length - 2; i >= 0; i--) {
        if (parts[i].trim().endsWith('市')) {
            city = parts[i].trim();
            break;
        }
    }
    airportCityElement.value = city;

    modal.style.display = 'block';
}

document.getElementById('confirmButton').addEventListener('click', function() {
    var name = airportNameInput.value;
    var country = airportCountryElement.value;
    var city = airportCityElement.value;
    var lat = airportLatInput.value;
    var lng = airportLngInput.value;

    if (window.qt_addAirport) {
        window.qt_addAirport.receiveAirportData(name, country, city, parseFloat(lat), parseFloat(lng));
    }

    modal.style.display = 'none';
});

document.getElementById('cancelButton').addEventListener('click', function() {
    modal.style.display = 'none';
});

window.onclick = function(event) {
    if (event.target === modal) {
        modal.style.display = 'none';
    }
};

document.addEventListener('click', function(event) {
    if (!suggestionsElement.contains(event.target) && event.target !== document.getElementById('airportSearch')) {
        suggestionsElement.style.display = 'none';
    }
});

document.getElementById('airportSearch').addEventListener('focus', function() {
    if (suggestionsElement.childNodes.length > 0) {
        suggestionsElement.style.display = 'block';
    }
});
