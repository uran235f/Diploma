// Initialize and add the map
let map;
let userLatitude = localStorage.getItem('latitude')
let userLongitude = localStorage.getItem('longitude')


async function initMap(text) {
    const locations = parseJSONString(text);
    const centerPosition = { lat: +userLatitude, lng: +userLongitude };
    //@ts-ignore
    const { Map } = await google.maps.importLibrary("maps");
    map = new Map(document.getElementById("map"), {
        zoom: 15,
        center: centerPosition,
        mapId: "DEMO_MAP_ID",
    });
    
    //const directionsService = new google.maps.DirectionsService();
    const directionsRenderer = new google.maps.DirectionsRenderer();
    // directionsRenderer.setMap(map);

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


function sendRequest() {
    const url = 'http://localhost:8080';

    let latitude = localStorage.getItem('latitude');
    let longitude = localStorage.getItem('longitude');
    let facilityType = localStorage.getItem('facilityType');
    let transportType = localStorage.getItem('transportType');

    let data = {
        latitude: latitude,
        longitude: longitude,   
        medicalFacility: facilityType,
        routeType: transportType
    };
    
    fetch(url, {
            method: 'POST',
            headers: {'Content-Type': 'application/json'},
            body: JSON.stringify(data)
        })
        .then(response => response.text())
        .then(text => initMap(text))
        .catch(error => console.error('There was an error with the request:', error));
}
    