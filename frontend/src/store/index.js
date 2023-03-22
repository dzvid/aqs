import api from '@/services';
import { format, parseISO } from 'date-fns';
import Vue from 'vue';
import Vuex from 'vuex';

Vue.use(Vuex);

const store = new Vuex.Store({
  state: {
    date: null,
    sensors: [],
    sensorReadings: [],
    aiqData: null,
    // {
    //   aiq: {
    //     aiq: null,
    //     index_color: '',
    //     index_classification: '',
    //     pollutant: '',
    //   },
    //   pm25: {
    //     aiq: null,
    //     index_color: '',
    //     index_classification: '',
    //     pollutant: '',
    //   },
    //   pm10: {
    //     aiq: null,
    //     index_color: '',
    //     index_classification: '',
    //     pollutant: '',
    //   },
    //   interval: {
    //     start_date: '',
    //     end_date: '',
    //   },
    // },
  },
  getters: {
    dataLabels: (state) =>
      state.sensorReadings
        .filter((reading) => reading.object !== null)
        .map((reading) => {
          const utcDate = reading.object.data.dt_collected_at;

          const date = new Date(utcDate).toLocaleDateString('pt-BR');
          const hour = new Date(utcDate).toLocaleTimeString('pt-BR');

          return `${date} ${hour}`;
        }),
    pm25Data: (state) =>
      state.sensorReadings
        .filter((reading) => reading.object !== null)
        .map((reading) => reading.object.data.pm2_5),
    pm10Data: (state) =>
      state.sensorReadings
        .filter((reading) => reading.object !== null)
        .map((reading) => reading.object.data.pm10),
    temperatureData: (state) =>
      state.sensorReadings
        .filter((reading) => reading.object !== null)
        .map((reading) => reading.object.data.temperature),
    humidityData: (state) =>
      state.sensorReadings
        .filter((reading) => reading.object !== null)
        .map((reading) => reading.object.data.humidity),
    pressureData: (state) =>
      state.sensorReadings
        .filter((reading) => reading.object !== null)
        .map((reading) => reading.object.data.pressure),

    dateFormatted: (state) =>
      state.date ? format(parseISO(state.date), 'dd/MM/yyyy') : state.date,
    dateISO: (state) =>
      state.date ? parseISO(state.date).toISOString() : state.date,
  },
  mutations: {
    SET_STATE_BY_KEY(state, { key, value }) {
      state[key] = value;
    },
    SET_SENSORS(state, payload) {
      state.sensors = payload;
    },
    SET_SENSOR_READINGS(state, payload) {
      state.sensorReadings = payload;
    },
  },
  actions: {
    setStateByKey({ commit }, payload) {
      commit('SET_STATE_BY_KEY', payload);
    },
    async fetchSensors({ commit }) {
      const { results } = await api.sensors.getSensors();
      commit('SET_SENSORS', results);
    },
    async fetchReadingsBySensorId({ commit }, payload) {
      const data = await api.sensors.getReadingsBySensorId(payload);
      commit('SET_SENSOR_READINGS', data);
    },
    async fetchAIQLast24Hours({ commit }, payload) {
      const data = await api.sensors.getAIQLast24Hours(payload);
      commit('SET_STATE_BY_KEY', { key: 'aiqData', value: data });
    },
  },
});

export default store;
