let map;
let clickedLat;
let clickedLng;

async function initSimpleMap() {
    const facilityType = localStorage.getItem('facilityType');
    const transportType = localStorage.getItem('transportType');
    const centerPosition = { lat: +localStorage.getItem('latitude'), lng: +localStorage.getItem('longitude') };
    //@ts-ignore
    const { Map } = await google.maps.importLibrary("maps");
    map = new Map(document.getElementById("map"), {
        zoom: 15,
        center: centerPosition,
        mapId: "DEMO_MAP_ID",
    });
    map.addListener('click', function(event) {
        clickedLat = event.latLng.lat();
        clickedLng = event.latLng.lng();
        document.getElementById('coordinates').innerHTML = 'Координати: ' + clickedLat + ', ' + clickedLng;
        document.getElementById('types').innerHTML = 'Типи: ' + facilityType + ', ' + transportType;
    });
}

async function sendRequest() {
    const url = 'http://localhost:8080';
    // const latitude = localStorage.getItem('latitude');
    // const longitude = localStorage.getItem('longitude');
    const facilityType = localStorage.getItem('facilityType');
    const transportType = localStorage.getItem('transportType');

    console.log("clickedLat", clickedLat)
    console.log("clickedLng", clickedLng)

    const data = {
        latitude: clickedLat,
        longitude: clickedLng,   
        medicalFacility: facilityType,
        routeType: transportType
    };

    console.log("data " + data.routeType);
    
    try {
        const response = await fetch(url, {
            method: 'POST',
            headers: {'Content-Type': 'application/json'},
            body: JSON.stringify(data)
        });
        console.log("here1")
        const text = await response.text();
        console.log("response text " + text);
        await addRouteToExistingMap(text);
    } catch (error) {
        console.error('There was an error with the request:', error);
    }
}

async function addRouteToExistingMap(text) {
    const locations = parseJSONString(text);
    
    const waypoints = locations.slice(0, -1).map(location => {
        return { lat: parseFloat(location.lat), lng: parseFloat(location.lon) };
    });
    
    map.fitBounds({
        west: Math.min.apply(null, waypoints.map(point => point.lng)),
        east: Math.max.apply(null, waypoints.map(point => point.lng)),
        north: Math.min.apply(null, waypoints.map(point => point.lat)),
        south: Math.max.apply(null, waypoints.map(point => point.lat)),
    });
    
    for (var i = 0; i < waypoints.length; i++) {
        new google.maps.Marker({
            position: waypoints[i],
            map: map,
            title: waypoints[i].name
        });
    }

    var stations = [];
    for (var i = 0; i < waypoints.length; i++) {
        console.log("parse lat " + parseFloat(waypoints[i].lat) + " lon " + parseFloat(waypoints[i].lng));
        var loc = new google.maps.LatLng(parseFloat(waypoints[i].lat),parseFloat(waypoints[i].lng));
        stations.push(loc);
    }
    var flightPath = new google.maps.Polyline({
        path: stations,
        geodesic: true,
        strokeColor: "#FF0000",
        strokeOpacity: 1.0,
        strokeWeight: 10
    });
    flightPath.setMap(map);
}

function parseJSONString(jsonString) {
    try {
        const data = JSON.parse(jsonString);
        if (Array.isArray(data)) {
            return data;
        } else {
            console.error("Input is not an array.");
            return [];
        }
    } catch (error) {
        console.error("Error parsing JSON:", error);
        return [];
    }
}
