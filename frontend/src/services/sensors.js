import httpClient from '@/plugins/axios';

async function getSensors() {
  const { data } = await httpClient.get('/sensordata/list_sensors/');
  return data;
}

async function getReadingsBySensorId({ idSensor, date }) {
  const { data } = await httpClient.get(
    `/sensordata/?sensor=${idSensor}&date=${date}`
  );
  return data;
}

export default {
  getSensors,
  getReadingsBySensorId,
};
