const char *HTML_CONTENT = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <title>WSS ESP32 Local</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.7.1/jquery.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/js/bootstrap.bundle.min.js"></script>
</head>

<body>
    <div class="page-wrapper" id="main-wrapper" data-layout="vertical">
        <div class="body-wrapper">
            <div class="container-fluid p-5">
                <div class="card mb-2 ">
                    <div class="card-header">Sensors and Actuators</div>
                    <div class="card-body bg-light">
                        <div class="row d-flex justify-content-center mb-5">
                            <h1 class=" text-center w-100">WSS ESP32 Local <button type="button w-50"
                                    class="btn btn-outline-primary"
                                    onclick="function reload(){ window.location = window.location.pathname;} reload()">Reload</button>
                            </h1>
                        </div>
                        <div class="row d-flex justify-content-center">
                            <div class="col-sm-6 col-xl-3">
                                <div class="card text-white bg-secondary mb-3">
                                    <div class="card-header">Temperature</div>
                                    <div class="card-body">
                                        <h5 id="temperature" class="card-title display-3">{{temperature}} °C</h5>
                                        <p class="card-text">Current temperature is shown here. It is degrees celsius
                                            using DHT22 sensor </p>
                                    </div>
                                </div>
                            </div>
                            <div class="col-sm-6 col-xl-3">
                                <div class="card text-white bg-info mb-3">
                                    <div class="card-header">Humididty</div>
                                    <div class="card-body">
                                        <h5 class="card-title display-3" id="humidity">{{humidity}} %</h5>
                                        <p class="card-text">Current humidity is shows here. It is in percent format
                                            using DHT22 sensor </p>
                                    </div>
                                </div>
                            </div>
                            <div class="col-sm-6 col-xl-3">
                                <div class="card text-white bg-warning mb-3">
                                    <div class="card-header">Light</div>
                                    <div class="card-body">
                                        <h5 id="light" class="card-title display-3 text-dark">{{light}}</h5>
                                        <p class="card-text">Environment brightness is shown here. LDR sensor is used to
                                            abtain this data</p>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="row d-flex justify-content-center">
                            <div class="col-sm-6 col-xl-3">
                                <div class="card bg-light mb-3">
                                    <div class="card-header">Relay</div>
                                    <div class="card-body">
                                        <h5 class="card-title">Relay is <b id="relay"
                                                class="display-3 font-weight-bold">{{relay}}</b>
                                        </h5>
                                        <p class="card-text">Current status of relay is shown here. </p>
                                        <div class="d-flex justify-content-center">
                                            <a href="?relay=1" class="btn btn-primary m-2">Turn On</a>
                                            <a href="?relay=0" class="btn btn-danger m-2">Turn Off</a>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="card bg-light mb-3">
                    <div class="card-header">WiFi Connection</div>
                    <div class="card-body">
                        <div class="row d-flex justify-content-center">
                            <div class="col-sm-8">
                                <h4 id="wifiStat" class=" text-left w-100">ESP32 is {{wifiStat}} </h1>
                                    <form>
                                        <div class="form-group p-2">
                                            <label for="ssid">WiFi SSID</label>
                                            <input type="text" name="ssid" class="form-control" id="ssid"
                                                placeholder="Enter SSID of network">
                                        </div>
                                        <div class="form-group p-2">
                                            <label for="pass">Password</label>
                                            <input type="password" name="password" class="form-control" id="pass"
                                                placeholder="Enter password of network">
                                        </div>
                                        <button type="submit" class="m-2 btn btn-primary">Connect</button>
                                        <a href="?scan=1" class="btn btn-success m-2">Scan</a>

                                    </form>
                                    <ul class="list-group  list-group-numbered">
                                        {{wifiNetworks}}
                                    </ul>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</body>
</html>
)=====";
