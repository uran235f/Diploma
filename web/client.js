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
    
    const directionsService = new google.maps.DirectionsService();
    const directionsRenderer = new google.maps.DirectionsRenderer();
    directionsRenderer.setMap(map);

    const waypoints = locations.slice(0, -1).map(location => {
        return { lat: parseFloat(location.lat), lng: parseFloat(location.lon) };
    });

    const request = {
        origin: waypoints[0],
        destination: waypoints[waypoints.length - 1],
        travelMode: google.maps.TravelMode.DRIVING,
        waypoints: waypoints.slice(1, -1).map(waypoint => ({
            location: waypoint,
            stopover: true
        }))
    };

    directionsService.route(request, function(result, status) {
        if (status == 'OK') {
            directionsRenderer.setDirections(result);

            const startMarker = new google.maps.Marker({
                position: result.routes[0].legs[0].start_location,
                map: map,
                title: 'Start'
            });
            const endMarker = new google.maps.Marker({
                position: result.routes[0].legs[result.routes[0].legs.length - 1].end_location,
                map: map,
                title: 'End'
            });
        }
    });
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
