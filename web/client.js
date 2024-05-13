let map;
let clickedLat;
let clickedLng;

async function initSimpleMap() {
    const facilityType = localStorage.getItem('facilityType');
    const transportType = localStorage.getItem('transportType');
    const centerPosition = { lat: 50.45379508376816, lng: 30.62703245899965 };
    //@ts-ignore
    const { Map } = await google.maps.importLibrary("maps");
    map = new Map(document.getElementById("map"), {
        zoom: 15,
        center: centerPosition,
        mapId: "DEMO_MAP_ID",
    });
    map.addListener('click', function (event) {
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
        medicalFacility: parseInt(facilityType),
        routeType: parseInt(transportType)
    };

    console.log("data " + data.medicalFacility);

    try {
        const response = await fetch(url, {
            method: 'delete',
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(data)
        });
        const text = await response.text();
        console.log("response text " + text);
        await addRouteToExistingMap(text);
    } catch (error) {
        console.error('There was an error with the request:', error);
    } finally {
        clickedLng = null;
        clickedLng = null;
    }
}

async function addRouteToExistingMap(text) {
    const locations = parseJSONString(text);
    const waypoints = locations.slice(0, -1).map(location => {
        console.log("lat: " + location.lat + " parsed " + parseFloat(location.lat));
        return { lat: parseFloat(location.lat), lng: parseFloat(location.lon), transport: location.transport };
    });
    console.log("waypoints", waypoints)

    map.fitBounds({
        west: Math.min.apply(null, waypoints.map(point => point.lng)),
        east: Math.max.apply(null, waypoints.map(point => point.lng)),
        north: Math.min.apply(null, waypoints.map(point => point.lat)),
        south: Math.max.apply(null, waypoints.map(point => point.lat)),
    });

    //for (var i = 0; i < waypoints.length; i++) {
    //    new google.maps.Marker({
    //        position: waypoints[i],
    //        map: map,
    //        title: waypoints[i].name
    //    });
    //}

    var stations = [];
    var trans = [];
    for (var i = 0; i < waypoints.length; i++) {
        //console.log("parse lat " + parseFloat(waypoints[i].lat) + " lon " + parseFloat(waypoints[i].lng));
        var loc = new google.maps.LatLng(parseFloat(waypoints[i].lat), parseFloat(waypoints[i].lng));
        trans.push(waypoints[i].transport)
        stations.push(loc);
    }
    console.log("trans", trans)

    const transportColors = {
        0: "#ea4335", // u-bahn red
        1: "#34a853", // public green
        2: "#673ab7", // car blue
        3: "#fbbc05"  // pedestrian orange
    };

    for (let i = 0; i < waypoints.length - 1; i++) {
        const routeColor = transportColors[waypoints[i].transport];
        const flightPath = new google.maps.Polyline({
            path: [stations[i], stations[i + 1]],
            geodesic: true,
            strokeColor: routeColor,
            strokeOpacity: 1.0,
            strokeWeight: 10
        });
        flightPath.setMap(map);
    }
}

function parseJSONString(jsonString) {
    try {
        const data = JSON.parse(jsonString);
        console.log("data " + data + " is array " + Array.isArray(data) + " type " + typeof(data));
        if (Array.isArray(data)) {
            console.log(data);
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
