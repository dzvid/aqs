import axios from 'axios';

const httpClient = axios.create({
  baseURL: process.env.VUE_APP_API_BASE_URL || 'http://192.168.0.11:9000/api',
  responseType: 'json',
  headers: {
    'Content-Type': 'application/json',
  },
});

export default httpClient;