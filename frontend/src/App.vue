<template>
  <v-app>
    <v-app-bar app color="secondary" flat>
      <h1>
        <span class="text-h3 text-bold white--text">AQS</span>
        <span class="text-h3 primary--text">UEA</span>
      </h1>
    </v-app-bar>

    <v-main class="">
      <v-container class="">
        <v-row dense>
          <v-col cols="12">
            <v-card class="" outlined>
              <v-card-title class="text-h5"> Selecione um sensor </v-card-title>
              <v-card-text>
                <v-form ref="form" lazy-validation>
                  <v-row>
                    <v-col cols="12" sm="4" md="3" lg="2">
                      <v-select
                        v-model="sensor"
                        :items="sensors"
                        label="Sensores"
                        item-value="device_name"
                        item-text="device_name"
                        outlined
                        hide-details="auto"
                        required
                        :rules="sensorRules"
                      ></v-select>
                    </v-col>
                    <v-col cols="12" sm="4" md="3" lg="2">
                      <v-menu
                        v-model="menu2"
                        :close-on-content-click="false"
                        :nudge-right="40"
                        transition="scale-transition"
                        offset-y
                        min-width="auto"
                      >
                        <template v-slot:activator="{ on, attrs }">
                          <v-text-field
                            v-model="dateFormatted"
                            label="Dia monitorado"
                            prepend-icon="mdi-calendar"
                            readonly
                            v-bind="attrs"
                            v-on="on"
                            @blur="date = parseDate(dateFormatted)"
                            :rules="dateRules"
                            hide-details="auto"
                            outlined
                            required
                          ></v-text-field>
                        </template>
                        <v-date-picker
                          v-model="date"
                          @input="menu2 = false"
                        ></v-date-picker>
                      </v-menu>
                    </v-col>
                    <v-col cols="12" sm="auto">
                      <v-btn
                        class="primary"
                        depressed
                        x-large
                        @click="onFetchReadingsBySensorId"
                      >
                        {{ 'Buscar' }}
                      </v-btn>
                    </v-col>
                  </v-row>
                </v-form>
              </v-card-text>
            </v-card>
          </v-col>
        </v-row>

        <v-row dense class="mt-4">
          <v-col cols="12">
            <v-card class="mx-auto" outlined>
              <v-card-title class="text-h5">
                Índice da Qualidade do Ar - IQA
              </v-card-title>

              <v-card-text>
                <v-row align="center">
                  <v-col class="text-h2" cols="6"> 23&deg;C </v-col>
                  <v-col cols="6">
                    <v-img
                      src="https://cdn.vuetifyjs.com/images/cards/sun.png"
                      alt="Sunny image"
                      width="92"
                    ></v-img>
                  </v-col>
                </v-row>
              </v-card-text>
            </v-card>
          </v-col>
        </v-row>

        <v-row dense>
          <v-col cols="12" md="4">
            <v-card class="mx-auto" flat outlined>
              <v-card-title> PM 2.5 </v-card-title>
              <v-card-text>
                <LineChart
                  :chart-options="chartOptions"
                  :chart-data="chartData"
                />
              </v-card-text>
            </v-card>
          </v-col>
        </v-row>

        <!-- <v-row dense>
          <v-col cols="12">
            <v-card class="mx-auto" flat>
              <v-list-item two-line>
                <v-list-item-content>
                  <v-list-item-title class="text-h5">
                    Temperatura
                  </v-list-item-title>
                </v-list-item-content>
              </v-list-item>
            </v-card>
          </v-col>
        </v-row>

        <v-row dense>
          <v-col cols="12">
            <v-card class="mx-auto" flat>
              <v-list-item two-line>
                <v-list-item-content>
                  <v-list-item-title class="text-h5">
                    Umidade
                  </v-list-item-title>
                </v-list-item-content>
              </v-list-item>
            </v-card>
          </v-col>
        </v-row> -->
      </v-container>
    </v-main>
  </v-app>
</template>

<script>
import { mapActions, mapState } from 'vuex';
import LineChart from '@/components/LineChart.vue';

export default {
  name: 'App',
  components: { LineChart },
  async created() {
    await this.fetchSensors();
  },
  methods: {
    ...mapActions(['fetchSensors', 'fetchReadingsBySensorId']),
    formatDate(date) {
      if (!date) return null;

      const [year, month, day] = date.split('-');
      return `${day}/${month}/${year}`;
    },
    parseDate(date) {
      if (!date) return null;

      const [day, month, year] = date.split('/');
      return `${year}-${month.padStart(2, '0')}-${day.padStart(2, '0')}`;
    },
    async onFetchReadingsBySensorId() {
      if (this.$refs.form.validate()) {
        await this.fetchReadingsBySensorId({
          idSensor: this.sensor,
          date: this.date,
        });
      }
    },
  },
  computed: {
    ...mapState(['sensors']),
    computedDateFormatted() {
      return this.formatDate(this.date);
    },
  },
  watch: {
    date() {
      this.dateFormatted = this.formatDate(this.date);
    },
  },
  data() {
    return {
      date: new Date(Date.now() - new Date().getTimezoneOffset() * 60000)
        .toISOString()
        .substr(0, 10),
      dateFormatted: this.formatDate(
        new Date(Date.now() - new Date().getTimezoneOffset() * 60000)
          .toISOString()
          .substr(0, 10)
      ),
      menu: false,
      modal: false,
      menu2: false,
      sensor: null,
      sensorRules: [(v) => !!v || 'Necessário selecionar um sensor.'],
      dateRules: [(v) => !!v || 'Necessário informar uma data.'],
      chartData: {
        labels: [
          'January',
          'February',
          'March',
          'April',
          'May',
          'June',
          'July',
        ],
        datasets: [
          {
            label: 'PM 2.5',
            backgroundColor: '#f87979',
            data: [10, 39, 10, 40, 39, 80, 40],
          },
        ],
      },
      chartOptions: {
        responsive: true,
        maintainAspectRatio: true,
        scales: {
          y: {
            min: 0,
            max: 600,
          },
          x: {
            min: 0,
          },
        },
      },
    };
  },
};
</script>
