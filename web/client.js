function sendRequest() {

    const url = 'http://localhost:8080';

    let facilityType = localStorage.getItem('facilityType');
    let transportType = localStorage.getItem('transportType');

    let data = {
        medicalFacility: facilityType,
        routeType: transportType
    };

    fetch(url, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(data)
        })
        .then(response => response.text())
        .then(text => {
            console.log(text); // Виведення відповіді у консоль
        })
        .catch(error => console.error('There was an error with the request:', error));

    //localStorage.clear();

}