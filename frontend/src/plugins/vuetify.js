import Vue from 'vue';
import Vuetify from 'vuetify/lib/framework';

Vue.use(Vuetify);

export default new Vuetify({
  theme: {
    light: {
      primary: '#673ab7',
      secondary: '#607d8b',
      accent: '#e91e63',
      error: '#f44336',
      warning: '#ff9800',
      info: '#03a9f4',
      success: '#4caf50',
    },
  },
});
