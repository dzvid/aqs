import api from '@/services';
import Vue from 'vue';
import Vuex from 'vuex';

Vue.use(Vuex);

const store = new Vuex.Store({
  state: {
    sensors: [],
    sensorReadings: [],
  },
  getters: {
    pm25Data: (state) =>
      state.sensorReadings
        .filter((reading) => reading.object !== null)
        .map((reading) => reading.object.data.pm2_5),
    pm25Labels: (state) =>
      state.sensorReadings
        .filter((reading) => reading.object !== null)
        .map((reading) => {
          const utcDate = `${reading.object.data.dt_collected_at}Z`;

          const date = new Date(utcDate).toLocaleDateString('pt-BR');
          const hour = new Date(utcDate).toLocaleTimeString('pt-BR');

          return `${date} ${hour}`;
        }),
  },
  mutations: {
    SET_SENSORS(state, payload) {
      state.sensors = payload;
    },
    SET_SENSOR_READINGS(state, payload) {
      state.sensorReadings = payload;
    },
  },
  actions: {
    async fetchSensors({ commit }) {
      const { results } = await api.sensors.getSensors();
      commit('SET_SENSORS', results);
    },
    async fetchReadingsBySensorId({ commit }, payload) {
      const { results } = await api.sensors.getReadingsBySensorId(payload);
      commit('SET_SENSOR_READINGS', results);
    },
  },
});

export default store;
