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
                            :value="dateFormatted"
                            label="Dia monitorado"
                            prepend-icon="mdi-calendar"
                            readonly
                            v-bind="attrs"
                            v-on="on"
                            :rules="dateRules"
                            hide-details="auto"
                            outlined
                            required
                          ></v-text-field>
                        </template>
                        <v-date-picker
                          :value="date"
                          @input="
                            (value) => {
                              setStateByKey({ key: 'date', value });
                              menu2 = false;
                            }
                          "
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
                  <v-col cols="6"> 23&deg;C</v-col>
                </v-row>
              </v-card-text>
            </v-card>
          </v-col>
        </v-row>

        <v-row dense>
          <v-col cols="12">
            <v-card class="mx-auto" flat outlined>
              <v-card-title> PM 2.5 (µg/m³) </v-card-title>
              <v-card-text>
                <div style="width: 100%; overflow-x: auto; overflow-y: hidden">
                  <div style="width: 625rem">
                    <LineChart
                      :chartOptions="chartOptions"
                      :chartData="chartDataPM25"
                    />
                  </div>
                </div>
              </v-card-text>
            </v-card>
          </v-col>
        </v-row>

        <v-row dense>
          <v-col cols="12">
            <v-card class="mx-auto" flat outlined>
              <v-card-title> PM 10 (µg/m³) </v-card-title>
              <v-card-text>
                <div style="width: 100%; overflow-x: auto; overflow-y: hidden">
                  <div style="width: 625rem">
                    <LineChart
                      :chartOptions="chartOptions"
                      :chartData="chartDataPM10"
                    />
                  </div>
                </div>
              </v-card-text>
            </v-card>
          </v-col>
        </v-row>

        <v-row dense>
          <v-col cols="12">
            <v-card class="mx-auto" flat outlined>
              <v-card-title> Temperatura (°C) </v-card-title>
              <v-card-text>
                <div style="width: 100%; overflow-x: auto; overflow-y: hidden">
                  <div style="width: 625rem">
                    <LineChart
                      :chartOptions="chartOptions"
                      :chartData="chartDataTemperature"
                    />
                  </div>
                </div>
              </v-card-text>
            </v-card>
          </v-col>
        </v-row>

        <v-row dense>
          <v-col cols="12">
            <v-card class="mx-auto" flat outlined>
              <v-card-title> Humidade Relativa (%) </v-card-title>
              <v-card-text>
                <div style="width: 100%; overflow-x: auto; overflow-y: hidden">
                  <div style="width: 625rem">
                    <LineChart
                      :chartOptions="chartOptions"
                      :chartData="chartDataHumidity"
                    />
                  </div>
                </div>
              </v-card-text>
            </v-card>
          </v-col>
        </v-row>

        <v-row dense>
          <v-col cols="12">
            <v-card class="mx-auto" flat outlined>
              <v-card-title> Pressão atmosférica (hPa) </v-card-title>
              <v-card-text>
                <div style="width: 100%; overflow-x: auto; overflow-y: hidden">
                  <div style="width: 625rem">
                    <LineChart
                      :chartOptions="chartOptions"
                      :chartData="chartDataPressure"
                    />
                  </div>
                </div>
              </v-card-text>
            </v-card>
          </v-col>
        </v-row>
      </v-container>
    </v-main>
  </v-app>
</template>

<script>
import { mapActions, mapGetters, mapState } from 'vuex';
import LineChart from '@/components/LineChart.vue';

export default {
  name: 'App',
  components: { LineChart },
  async mounted() {
    await this.fetchSensors();
  },
  methods: {
    ...mapActions(['fetchSensors', 'fetchReadingsBySensorId', 'setStateByKey']),
    async onFetchReadingsBySensorId() {
      try {
        this.$vloading.show();
        if (this.$refs.form.validate()) {
          await this.fetchReadingsBySensorId({
            idSensor: this.sensor,
            date: this.dateISO,
          });
        }
      } catch (error) {
        console.warn(error);
      } finally {
        this.$vloading.hide();
      }
    },
  },
  computed: {
    ...mapState(['sensors', 'date']),
    ...mapGetters([
      'dataLabels',
      'pm25Data',
      'pm10Data',
      'temperatureData',
      'humidityData',
      'pressureData',
      'dateFormatted',
      'dateISO',
    ]),
    chartDataPM25() {
      return {
        labels: this.dataLabels,
        datasets: [
          {
            label: 'PM 2.5',
            backgroundColor: '#f87979',
            data: this.pm25Data,
          },
        ],
      };
    },
    chartDataPM10() {
      return {
        labels: this.dataLabels,
        datasets: [
          {
            label: 'PM 10',
            backgroundColor: '#efaafe',
            data: this.pm10Data,
          },
        ],
      };
    },
    chartDataTemperature() {
      return {
        labels: this.dataLabels,
        datasets: [
          {
            label: 'Temperatura',
            backgroundColor: '#efaafe',
            data: this.temperatureData,
          },
        ],
      };
    },
    chartDataHumidity() {
      return {
        labels: this.dataLabels,
        datasets: [
          {
            label: 'Humidade',
            backgroundColor: '#efaafe',
            data: this.humidityData,
          },
        ],
      };
    },
    chartDataPressure() {
      return {
        labels: this.dataLabels,
        datasets: [
          {
            label: 'Pressão atmosférica',
            backgroundColor: '#efaafe',
            data: this.pressureData,
          },
        ],
      };
    },
    chartOptions() {
      return {
        responsive: true,
        maintainAspectRatio: false,
        scales: {
          y: {
            min: 0,
          },
          x: {
            min: 0,
          },
        },
      };
    },
  },
  data() {
    return {
      menu: false,
      modal: false,
      menu2: false,
      sensor: null,
      sensorRules: [(v) => !!v || 'Necessário selecionar um sensor.'],
      dateRules: [(v) => !!v || 'Necessário informar uma data.'],
    };
  },
};
</script>
