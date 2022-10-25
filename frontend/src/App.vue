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
              <v-card-title class="text-h5">
                Índice de Qualidade do Ar (IQAr)
              </v-card-title>
              <v-card-subtitle class="text-subtitle-1 mt-2">
                Selecione uma unidade de monitoramento para obter o IQAr
                mensurado nas últimas 24 horas:
              </v-card-subtitle>
              <v-card-text>
                <v-form ref="formAiq" lazy-validation>
                  <v-row>
                    <v-col cols="12" sm="4" md="3" lg="2">
                      <v-select
                        v-model="sensorAiq"
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
                    <v-col cols="12" sm="auto">
                      <v-btn
                        class="primary"
                        depressed
                        x-large
                        @click="onFetchAiqBySensorId"
                      >
                        {{ 'Consultar' }}
                      </v-btn>
                    </v-col>
                  </v-row>
                </v-form>

                <v-row dense class="mt-4" v-if="aiqData !== null">
                  <v-col cols="12">
                    <v-row>
                      <v-col cols="12" md="6">
                        <v-card
                          :class="`${
                            getAiqClassification(
                              aiqData.aiq.index_classification
                            ).class
                          } pa-4 white--text`"
                          height="100%"
                        >
                          <v-card-title class="text-h1 font-weight-black">
                            <div>IQAr</div>
                          </v-card-title>
                          <v-card-subtitle
                            class="text-h1 font-weight-bold white--text mt-4"
                          >
                            <div>{{ aiqData.aiq.aiq }}</div>
                          </v-card-subtitle>
                          <v-card-text
                            class="text-h6 font-weight-medium d-inline-block white--text mt-4 d-flex align-center"
                          >
                            <v-row dense>
                              <v-col cols="12">
                                <span> Poluente: </span>
                                {{ getPollutant(aiqData.aiq.pollutant) }}
                              </v-col>
                              <v-col cols="12">
                                <span> Classificação: </span>
                                {{
                                  getAiqClassification(
                                    aiqData.aiq.index_classification
                                  ).text
                                }}
                              </v-col>
                            </v-row>

                            <v-spacer></v-spacer>

                            <v-dialog
                              transition="dialog-bottom-transition"
                              max-width="1200"
                            >
                              <template v-slot:activator="{ on, attrs }">
                                <v-btn
                                  icon
                                  color="white"
                                  v-bind="attrs"
                                  v-on="on"
                                  outlined
                                >
                                  <v-icon>mdi-help</v-icon>
                                </v-btn>
                              </template>
                              <template v-slot:default="dialog">
                                <v-card>
                                  <v-toolbar color="primary" dark
                                    >Relação entre o valor do IQAr e possíveis
                                    efeitos à saúde</v-toolbar
                                  >
                                  <v-card-text>
                                    <v-data-table
                                      :headers="headers"
                                      :items="descriptions"
                                      :items-per-page="5"
                                      disable-pagination
                                      disable-filtering
                                      disable-sort
                                      hide-default-footer
                                      :item-class="itemRowBackground"
                                    >
                                    </v-data-table>
                                  </v-card-text>
                                  <v-card-actions>
                                    <v-spacer></v-spacer>
                                    <v-btn
                                      icon
                                      @click="dialog.value = false"
                                      class="mr-2"
                                      >Fechar</v-btn
                                    >
                                  </v-card-actions>
                                </v-card>
                              </template>
                            </v-dialog>
                          </v-card-text>
                        </v-card>
                      </v-col>
                      <v-col cols="12" md="6">
                        <v-card height="100%">
                          <v-card-text>
                            <v-list dense disabled>
                              <v-subheader class="text-subtitle-1"
                                >Dados da estação monitoramento das últimas 24
                                horas</v-subheader
                              >
                              <ul style="list-style-type: none">
                                <li
                                  v-for="(item, i) in aqiItems"
                                  :key="i"
                                  dense
                                >
                                  <div
                                    class="d-flex justify-start align-start flex-column pa-2"
                                  >
                                    <div
                                      class="d-flex justify-start align-center"
                                    >
                                      <v-icon class="mr-2">{{
                                        item.icon
                                      }}</v-icon>
                                      <span class="text-subtitle-1 mr-2">
                                        {{ item.text }}:
                                      </span>
                                      <span>
                                        {{ item.value }}
                                      </span>
                                    </div>
                                  </div>
                                </li>
                              </ul>
                              <!-- <v-list-item-group color="primary">
                                <v-list-item
                                  v-for="(item, i) in aqiItems"
                                  :key="i"
                                  dense
                                >
                                  <v-list-item-icon>
                                    <v-icon>{{ item.icon }}</v-icon>
                                  </v-list-item-icon>
                                  <v-list-item-content>
                                    <v-list-item-title class="text-subtitle-2">
                                      {{ item.text }}
                                    </v-list-item-title>

                                    <v-list-item-subtitle
                                      class="text-subtitle-2"
                                    >
                                      {{ item.value }}
                                    </v-list-item-subtitle>
                                  </v-list-item-content>
                                </v-list-item>
                              </v-list-item-group> -->
                            </v-list>
                          </v-card-text>
                        </v-card>
                      </v-col>
                    </v-row>
                  </v-col>
                </v-row>
              </v-card-text>
            </v-card>
          </v-col>
        </v-row>

        <v-row dense>
          <v-col cols="12">
            <v-card class="" outlined>
              <v-card-title class="text-h5">
                Histórico de monitoramento diário por estação
              </v-card-title>
              <v-card-subtitle class="text-subtitle-1 mt-2">
                Selecione um sensor:
              </v-card-subtitle>
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

                <v-row dense>
                  <v-col cols="12">
                    <v-card class="mx-auto mt-4" flat outlined>
                      <v-card-title> PM 2.5 (µg/m³) </v-card-title>
                      <v-card-text>
                        <div
                          style="
                            width: 100%;
                            overflow-x: auto;
                            overflow-y: hidden;
                          "
                        >
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
                    <v-card class="mx-auto mt-4" flat outlined>
                      <v-card-title> PM 10 (µg/m³) </v-card-title>
                      <v-card-text>
                        <div
                          style="
                            width: 100%;
                            overflow-x: auto;
                            overflow-y: hidden;
                          "
                        >
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
                    <v-card class="mx-auto mt-4" flat outlined>
                      <v-card-title> Temperatura (°C) </v-card-title>
                      <v-card-text>
                        <div
                          style="
                            width: 100%;
                            overflow-x: auto;
                            overflow-y: hidden;
                          "
                        >
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
                    <v-card class="mx-auto mt-4" flat outlined>
                      <v-card-title> Humidade Relativa (%) </v-card-title>
                      <v-card-text>
                        <div
                          style="
                            width: 100%;
                            overflow-x: auto;
                            overflow-y: hidden;
                          "
                        >
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
                    <v-card class="mx-auto mt-4" flat outlined>
                      <v-card-title> Pressão atmosférica (hPa) </v-card-title>
                      <v-card-text>
                        <div
                          style="
                            width: 100%;
                            overflow-x: auto;
                            overflow-y: hidden;
                          "
                        >
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
    ...mapActions([
      'fetchSensors',
      'fetchReadingsBySensorId',
      'fetchAIQLast24Hours',
      'setStateByKey',
    ]),
    itemRowBackground(item) {
      return item.color;
    },
    getPollutant(pollutant) {
      if (pollutant === 'pm25') return 'MP 2.5';
      return 'MP 10';
    },
    getAiqClassification(aiqClassification) {
      if (aiqClassification === 'good') {
        return { class: 'green', text: 'N1 - Boa' };
      }
      if (aiqClassification === 'moderated') {
        return { class: 'yellow', text: 'N2 - Moderada' };
      }

      if (aiqClassification === 'bad') {
        return { class: 'orange', text: 'N3 - Ruim' };
      }

      if (aiqClassification === 'very_bad') {
        return { class: 'red', text: 'N4 - Muito Ruim' };
      }

      if (aiqClassification === 'terrible') {
        return { class: 'purple', text: 'N5 - Péssima' };
      }

      return { class: '', text: '' };
    },
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
    async onFetchAiqBySensorId() {
      try {
        this.$vloading.show();
        if (this.$refs.formAiq.validate()) {
          await this.fetchAIQLast24Hours({
            idSensor: this.sensorAiq,
            date: new Date().toISOString(),
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
    ...mapState(['sensors', 'date', 'aiqData']),
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
    aqiItems() {
      return [
        {
          text: 'Local de monitoramento',
          icon: 'mdi-map-marker',
          value: `Latitude: ${this.aiqData.location.latitude}, Longitude: ${this.aiqData.location.longitude}`,
        },
        {
          text: 'Intervalo de tempo',
          icon: 'mdi-calendar',
          value: `${new Date(this.aiqData.interval.end_date).toLocaleDateString(
            'pt-BR'
          )} ${new Date(this.aiqData.interval.end_date).toLocaleTimeString(
            'pt-BR'
          )} - ${new Date(this.aiqData.interval.start_date).toLocaleDateString(
            'pt-BR'
          )} ${new Date(this.aiqData.interval.start_date).toLocaleTimeString(
            'pt-BR'
          )}`,
        },
        {
          text: 'IQAr MP 2.5 µg/m³',
          icon: 'mdi-flag',
          value: this.aiqData.pm25.aiq,
        },
        {
          text: 'Classificação IQAr MP 2.5 µg/m³',
          icon: 'mdi-thermometer',
          value: this.getAiqClassification(
            this.aiqData.pm25.index_classification
          ).text,
        },
        {
          text: 'IQAr MP 10 µg/m³',
          icon: 'mdi-flag',
          value: this.aiqData.pm10.aiq,
        },
        {
          text: 'Classificação IQAr MP 10 µg/m³',
          icon: 'mdi-thermometer',
          value: this.getAiqClassification(
            this.aiqData.pm10.index_classification
          ).text,
        },
      ];
    },
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
      sensorAiq: null,
      sensor: null,
      sensorRules: [(v) => !!v || 'Necessário selecionar um sensor.'],
      dateRules: [(v) => !!v || 'Necessário informar uma data.'],
      headers: [
        {
          text: 'Qualidade do ar',
          align: 'start',
          sortable: false,
          value: 'aiq',
          class: 'text-truncate',
          cellClass: 'text-truncate',
        },
        {
          text: 'Intervalo do Indice (IQAr)',
          align: 'start',
          sortable: false,
          value: 'index',
          class: 'text-truncate',
          cellClass: 'text-truncate',
        },
        {
          text: 'Efeitos',
          align: 'start',
          sortable: false,
          value: 'effect',
          class: 'text-truncate',
        },
      ],
      descriptions: [
        {
          aiq: 'N1 - Boa',
          index: '0 - 40',
          effect: '-',
          color: 'green',
        },
        {
          aiq: 'N2 - Moderada',
          index: '41 - 80',
          effect:
            'Pessoas de grupos sensíveis (crianças, idosos e pessoas com doenças respiratórias e cardíacas) podem apresentar sintomas como tosse seca e cansaço. A população em geral não é afetada.',
          color: 'yellow',
        },
        {
          aiq: 'N3 - Ruim',
          index: '81 - 120',
          effect:
            'Toda a população pode apresentar sintomas como tosse seca, cansaço, ardor nos olhos, nariz e garganta. Pessoas de grupos sensíveis (crianças, idosos e pessoas com doenças respiratórias e cardíacas) podem apresentar efeitos mais sérios na saúde.',
          color: 'orange',
        },
        {
          aiq: 'N4 - Muito Ruim',
          index: '121 - 200',
          effect:
            'Toda a população pode apresentar agravamento dos sintomas como tosse seca, cansaço, ardor nos olhos, nariz e garganta e ainda falta de ar e respiração ofegante. Efeitos ainda mais graves à saúde de grupos sensíveis (crianças, idosos e pessoas com doenças respiratórias e cardíacas).',
          color: 'red',
        },
        {
          aiq: 'N5 - Péssima',
          index: '201 - 400',
          effect:
            'Toda a população pode apresentar sérios riscos de manifestações de doenças respiratórias e cardio-vasculares. Aumento de mortes prematuras em pessoas de grupos sensíveis.',
          color: 'purple',
        },
      ],
    };
  },
};
</script>

<style scoped></style>
