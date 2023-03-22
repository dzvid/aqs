import httpClient from '@/plugins/axios';

async function getSensors() {
  const { data } = await httpClient.get('/sensordata/list_sensors/');
  return data;
}

async function getReadingsBySensorId({ idSensor, date }) {
  const { data } = await httpClient.get(
    `/sensordata/get_readings/?id_sensor=${idSensor}&date=${date}`
  );
  return data;
}

async function getAIQLast24Hours({ idSensor, date }) {
  const { data } = await httpClient.get(
    `/sensordata/get_aqi_last24hrs/?id_sensor=${idSensor}&date=${date}`
  );
  return data;
}

export default {
  getSensors,
  getReadingsBySensorId,
  getAIQLast24Hours,
};
