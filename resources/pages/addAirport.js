var map = L.map('map').setView([39.9, 116.4], 5);

// 初始化 QWebChannel，确保与 Qt 进行通信
new QWebChannel(qt.webChannelTransport, function(channel) {
    window.qt = channel.objects.qt; // 注册 qt 对象
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

// 处理搜索输入并显示建议
document.getElementById('airportSearch').addEventListener('input', function() {
    var airportName = this.value;
    if (airportName) {
        fetch(`https://nominatim.openstreetmap.org/search?q=${airportName} airport&format=json`)
        // fetch(`https://nominatim.openstreetmap.org/search?city=${airportName}&format=json`)
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

// 显示机场位置和信息
function displayAirport(item) {
    var lat = item.lat;
    var lng = item.lon;

    // 清除之前的标记
    map.eachLayer(function(layer) {
        if (layer instanceof L.Marker) {
            map.removeLayer(layer);
        }
    });

    // 添加新的标记
    var marker = L.marker([lat, lng]).addTo(map);
    marker.bindPopup(item.display_name).openPopup();
    map.setView([lat, lng], 10);
    suggestionsElement.style.display = 'none';

    // 分割机场名称以获取城市和国家
    var parts = item.display_name.split(',');
    airportNameInput.value = parts[0].trim(); // 设置机场名称
    airportLatInput.value = lat; // 设置纬度
    airportLngInput.value = lng; // 设置经度
    var city = parts.length > 1 ? parts[parts.length - 2].trim() : '未知城市'; // 获取城市
    var country = parts.length > 2 ? parts[parts.length - 1].trim() : '未知国家'; // 获取国家
    airportCityElement.value = city; // 设置城市
    airportCountryElement.value = country; // 设置国家
    modal.style.display = 'block'; // 显示模态框
}

// 确认添加机场
document.getElementById('confirmButton').addEventListener('click', function() {
    var name = airportNameInput.value;
    var country = airportCountryElement.value;
    var city = airportCityElement.value;
    var lat = airportLatInput.value;
    var lng = airportLngInput.value;

    // 通过 QWebChannel 发送数据
    if (window.qt) {
        window.qt.receiveAirportData(name, country, city, parseFloat(lat), parseFloat(lng));
    }

    // 关闭模态框
    modal.style.display = 'none';
});

// 关闭模态框
document.getElementById('cancelButton').addEventListener('click', function() {
    modal.style.display = 'none';
});

// 点击模态框外部关闭模态框
window.onclick = function(event) {
    if (event.target === modal) {
        modal.style.display = 'none';
    }
};

// 点击其他地方隐藏建议框
document.addEventListener('click', function(event) {
    if (!suggestionsElement.contains(event.target) && event.target !== document.getElementById('airportSearch')) {
        suggestionsElement.style.display = 'none';
    }
});

// 点击搜索框显示建议框
document.getElementById('airportSearch').addEventListener('focus', function() {
    if (suggestionsElement.childNodes.length > 0) {
        suggestionsElement.style.display = 'block';
    }
});
