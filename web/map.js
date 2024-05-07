// Initialize and add the map
let map;
let userLatitude = localStorage.getItem('latitude')
let userLongitude = localStorage.getItem('longitude')

async function initMap(text) {
    const centerPosition = { lat: +userLatitude, lng: +userLongitude };
    //@ts-ignore
    const { Map } = await google.maps.importLibrary("maps");
    map = new Map(document.getElementById("map"), {
        zoom: 13,
        center: centerPosition,
        mapId: "DEMO_MAP_ID",
    });
    const directionsService = new google.maps.DirectionsService();
    const directionsRenderer = new google.maps.DirectionsRenderer();
    directionsRenderer.setMap(map);

    const waypoints = [
        { lat: 50.477410, lng: 30.621675 },
        { lat: 50.477440, lng: 30.621788 },
        { lat: 50.477452, lng: 30.621833 },
        { lat: 50.477531, lng: 30.621788 }
    ];
    const request = {
        origin: waypoints[0],
        destination: waypoints[waypoints.length - 1],
        travelMode: google.maps.TravelMode.DRIVING,
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

initMap();