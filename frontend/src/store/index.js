import api from '@/services';
import Vue from 'vue';
import Vuex from 'vuex';

Vue.use(Vuex);

const store = new Vuex.Store({
  state: {
    sensors: [],
    sensorReadings: null,
  },
  getters: {},
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
    async fetchReadingsBySensorId({ commit }) {
      const { results } = await api.sensors.getReadingsBySensorId();
      commit('SET_SENSOR_READINGS', results);
    },
  },
});

export default store;
