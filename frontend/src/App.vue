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
          <v-col cols="12">
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
import { mapActions, mapGetters, mapState } from 'vuex';
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
    ...mapGetters(['pm25Data', 'pm25Labels']),
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
          '16/10/2022 12:37:07',
          '16/10/2022 12:38:07',
          '16/10/2022 12:40:07',
          '17/10/2022 22:35:08',
          '17/10/2022 22:37:08',
          '16/10/2022 12:41:07',
          '16/10/2022 12:42:07',
          '16/10/2022 12:43:07',
          '16/10/2022 12:44:07',
          '16/10/2022 12:45:07',
          '16/10/2022 12:46:07',
          '16/10/2022 12:47:07',
          '16/10/2022 12:49:07',
          '16/10/2022 12:51:07',
          '16/10/2022 12:53:07',
          '16/10/2022 12:55:07',
          '16/10/2022 12:57:07',
          '16/10/2022 12:59:07',
          '16/10/2022 13:01:07',
          '16/10/2022 13:03:07',
          '16/10/2022 13:05:07',
          '16/10/2022 12:48:07',
          '16/10/2022 12:50:07',
          '16/10/2022 12:52:07',
          '16/10/2022 12:54:07',
          '16/10/2022 12:56:07',
          '16/10/2022 12:58:07',
          '16/10/2022 13:00:07',
          '16/10/2022 13:02:07',
          '16/10/2022 13:04:07',
          '16/10/2022 13:06:07',
          '16/10/2022 13:07:07',
          '16/10/2022 13:09:07',
          '16/10/2022 13:11:07',
          '16/10/2022 13:13:07',
          '16/10/2022 13:15:07',
          '16/10/2022 13:17:07',
          '16/10/2022 13:19:07',
          '16/10/2022 13:21:07',
          '16/10/2022 13:23:07',
          '17/10/2022 22:36:08',
          '17/10/2022 22:38:08',
          '16/10/2022 13:08:07',
          '16/10/2022 13:10:07',
          '16/10/2022 13:12:07',
          '16/10/2022 13:14:07',
          '16/10/2022 13:16:07',
          '16/10/2022 13:18:07',
          '16/10/2022 13:20:07',
          '16/10/2022 13:22:07',
          '16/10/2022 13:24:07',
          '16/10/2022 13:25:07',
          '16/10/2022 13:26:07',
          '16/10/2022 13:27:07',
          '16/10/2022 13:29:07',
          '16/10/2022 13:31:07',
          '16/10/2022 13:33:07',
          '16/10/2022 13:35:07',
          '16/10/2022 13:37:07',
          '16/10/2022 13:39:07',
          '16/10/2022 13:41:07',
          '16/10/2022 13:43:07',
          '16/10/2022 13:45:07',
          '16/10/2022 13:47:07',
          '16/10/2022 13:28:07',
          '16/10/2022 13:30:07',
          '16/10/2022 13:32:07',
          '16/10/2022 13:34:07',
          '16/10/2022 13:36:07',
          '16/10/2022 13:38:07',
          '16/10/2022 13:40:07',
          '16/10/2022 13:42:07',
          '16/10/2022 13:44:07',
          '16/10/2022 13:46:07',
          '16/10/2022 13:48:07',
          '16/10/2022 13:50:07',
          '16/10/2022 13:52:07',
          '16/10/2022 13:54:07',
          '16/10/2022 13:56:07',
          '16/10/2022 13:58:07',
          '16/10/2022 14:00:07',
          '16/10/2022 14:02:07',
          '16/10/2022 14:04:07',
          '16/10/2022 14:06:07',
          '16/10/2022 14:08:07',
          '16/10/2022 13:49:07',
          '16/10/2022 13:51:07',
          '16/10/2022 13:53:07',
          '16/10/2022 13:55:07',
          '16/10/2022 13:57:07',
          '16/10/2022 13:59:07',
          '16/10/2022 14:01:07',
          '16/10/2022 14:03:07',
          '16/10/2022 14:05:07',
          '16/10/2022 14:07:07',
          '16/10/2022 14:09:07',
          '16/10/2022 14:10:07',
        ],
        datasets: [
          {
            label: 'PM 2.5',
            backgroundColor: '#f87979',
            data: [
              20, 20, 22, 16, 15, 23, 23, 24, 25, 26, 27, 25, 25, 25, 25, 26,
              27, 27, 29, 18, 17, 25, 24, 25, 24, 27, 27, 29, 29, 18, 17, 18,
              19, 19, 19, 18, 18, 20, 19, 19, 16, 15, 18, 19, 19, 19, 18, 18,
              20, 19, 19, 17, 17, 17, 16, 17, 16, 15, 16, 16, 15, 16, 16, 17,
              17, 16, 17, 16, 15, 16, 16, 15, 16, 16, 17, 16, 13, 14, 14, 14,
              14, 15, 14, 13, 16, 16, 13, 14, 14, 14, 14, 15, 14, 13, 16, 15,
              15,
            ],
          },
        ],
      },
      chartOptions: {
        responsive: true,
        maintainAspectRatio: true,
        scales: {
          y: {
            min: 0,
            max: 500,
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
