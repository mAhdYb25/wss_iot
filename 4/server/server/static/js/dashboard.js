const seriesCount = 50;

const chart = {
  type: "area",
  height: 60,
  sparkline: {
    enabled: true,
  },
  group: "sparklines",
  fontFamily: "Plus Jakarta Sans', sans-serif",
};

const stroke = {
  curve: "smooth",
  width: 2,
};

const fill = {
  type: "solid",
  opacity: 0.05,
};

const markers = {
  size: 0,
};

const tooltip = {
  theme: "dark",
  fixed: {
    enabled: true,
    position: "right",
  },
  x: {
    show: false,
  },
};

function drawChart(id, name, color, data) {
  const options = {
    chart: { ...chart, id: `${id}-inner`, color },
    series: [{ name, color, data }],
    stroke,
    fill: { ...fill, colors: [color] },
    markers,
    tooltip,
  };

  new ApexCharts($(`#${id}`)[0], options).render();
}

function padArray(array, length, fill) {
  return length > array.length
    ? Array(length - array.length)
      .fill(fill)
      .concat(array)
    : array;
}

async function fetchData() {
  const response = await fetch("/mqtt/data/");
  const data = await response.json();
  return data;
}

$(async function () {
  $("#relay-input").change(async function () {
    const relay = $(this).is(":checked");
    const csrftoken = Cookies.get("csrftoken");
    const data = { relay };
    await fetch("/mqtt/relay/", {
      method: "POST",
      body: JSON.stringify(data),
      headers: {
        "Content-Type": "application/json",
        "X-CSRFToken": csrftoken,
        mode: "same-origin",
      },
    });

    $("#relay").html(relay ? "ON" : "OFF");
  });

  const { temperature, humidity, light, wifi, relay } = await fetchData();

  paddedTemperature = padArray(temperature, seriesCount, 0);
  paddedHumidity = padArray(humidity, seriesCount, 0);
  paddedLight = padArray(light, seriesCount, 0);

  drawChart("temperature-chart", "Temperature", "#E64A4A", paddedTemperature);
  drawChart("humidity-chart", "Humidity", "#5D87FF", paddedHumidity);
  drawChart("light-chart", "Light", "#FFAE1F", paddedLight);

  $("#temperature").html(`${paddedTemperature[seriesCount - 1]} °C`);
  $("#humidity").html(`${paddedHumidity[seriesCount - 1]} %`);
  
  const lightValue = paddedLight[seriesCount - 1];
  let lightVolume;
  if (lightValue < 1000) lightVolume = "Dark";
  else if (lightValue < 3000) lightVolume = "Normal";
  else lightVolume = "Bright";
  $("#light").html(`${lightValue} (${lightVolume})`);

  $("#wifi").html(wifi);
  $("#relay").html(relay ? "ON" : "OFF");
});
