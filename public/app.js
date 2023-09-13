// app.js
document.getElementById("scanBtn").addEventListener("click", function() {
    fetch('/scan').then(response => response.text()).then(data => {
        const networks = data.split("\n");
        const networkList = document.getElementById("networkList");
        networkList.innerHTML = '';
        networks.forEach(net => {
            const listItem = document.createElement("li");
            listItem.textContent = net;
            listItem.addEventListener("click", function() {
                document.getElementById("ssidInput").value = net;
            });
            networkList.appendChild(listItem);
        });
    });
});



document.getElementById("speedTestButton").addEventListener("click", function() {
    fetch('/speedTest').then(response => response.text()).then(data => {
        console.log(data);
        document.getElementById("speed").innerHTML = data;
    });
});


document.getElementById("netDetailsButton").addEventListener("click", function() {
    fetch('/netDetails').then(response => response.text()).then(data => {
        console.log(data);
        document.getElementById("netDetails").innerHTML = data;
    });
});


function getCurrentNetwork() {
    fetch('/currentNet').then(response => response.text()).then(data => {
        console.log(data);
        document.getElementById("currentNet").innerHTML = data;

    });
}
getCurrentNetwork();

document.getElementById("connectBtn").addEventListener("click", function() {
    const ssid = document.getElementById("ssidInput").value;
    
   
    const password = document.getElementById("passwordInput").value;
    
    fetch('/connect', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ ssid, password }),
    }).then(response => response.text()).then(data => {
        console.log(data);
        alert(data);
    });
});

document.getElementById("disconnectBtn").addEventListener("click", function() {
    fetch('/disconnect', {
        method: 'POST',
    }).then(response => response.text()).then(data => {
            alert(data);
    });
});