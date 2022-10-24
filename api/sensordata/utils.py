# Standard levels
n1_good_interval = {
    'upper_limit': 40,
    'lower_limit': 0,
    'color': 'green',
    'classification': 'good'
}

n2_moderated_interval = {
    'upper_limit': 80,
    'lower_limit': 41,
    'color': 'yellow',
    'classification': 'moderated'
}

n3_bad_interval = {
    'upper_limit': 120,
    'lower_limit': 81,
    'color': 'orange',
    'classification': 'bad'
}

n4_very_bad_interval = {
    'upper_limit': 200,
    'lower_limit': 121,
    'color': 'red',
    'classification': 'very_bad'
}

n5_terrible_interval = {
    'upper_limit': 400,
    'lower_limit': 201,
    'color': 'purple',
    'classification': 'terrible'
}

# PM10 levels
pm10_good_interval = {
    'upper_limit': 50,
    'lower_limit': 0,
    'color': 'green',
    'classification': 'good'
}

pm10_moderated_interval = {
    'upper_limit': 100,
    'lower_limit': 51,
    'color': 'yellow',
    'classification': 'moderated'
}

pm10_bad_interval = {
    'upper_limit': 150,
    'lower_limit': 101,
    'color': 'orange',
    'classification': 'bad'
}

pm10_very_bad_interval = {
    'upper_limit': 250,
    'lower_limit': 151,
    'color': 'red',
    'classification': 'very_bad'
}

pm10_terrible_interval = {
    'upper_limit': 600,
    'lower_limit': 251,
    'color': 'purple',
    'classification': 'terrible'
}

# PM25 levels
pm25_good_interval = {
    'upper_limit': 25,
    'lower_limit': 0,
    'color': 'green',
    'classification': 'good'
}

pm25_moderated_interval = {
    'upper_limit': 50,
    'lower_limit': 26,
    'color': 'yellow',
    'classification': 'moderated'
}

pm25_bad_interval = {
    'upper_limit': 75,
    'lower_limit': 51,
    'color': 'orange',
    'classification': 'bad'
}

pm25_very_bad_interval = {
    'upper_limit': 125,
    'lower_limit': 76,
    'color': 'red',
    'classification': 'very_bad'
}

pm25_terrible_interval = {
    'upper_limit': 300,
    'lower_limit': 126,
    'color': 'purple',
    'classification': 'terrible'
}


# Methods

def get_average(readings=[], pollutant=None):
    amount_readings = len(readings)
    if not amount_readings:
        return 0

    total_sum = 0
    for reading in readings:
        total_sum += reading[pollutant]

    return round(total_sum / amount_readings)


def calculate_aiq(i_initial=None, i_final=None, c_initial=None, c_final=None, c_measured=None):
    result_aiq = i_initial + ((i_final - i_initial) / (c_final - c_initial)) * (c_measured - c_initial)
    return result_aiq


def get_pm25_interval_and_concentration(pm25_average_measured_conc=None):
    if pm25_average_measured_conc <= pm25_good_interval['upper_limit']:
        return pm25_good_interval, n1_good_interval

    if pm25_average_measured_conc <= pm25_moderated_interval['upper_limit']:
        return pm25_moderated_interval, n2_moderated_interval

    if pm25_average_measured_conc <= pm25_bad_interval['upper_limit']:
        return pm25_bad_interval, n3_bad_interval

    if pm25_average_measured_conc <= pm25_very_bad_interval['upper_limit']:
        return pm25_very_bad_interval, n4_very_bad_interval

    return pm25_terrible_interval, n5_terrible_interval


def get_pm25_aiq(pm25_average_measured_conc):
    pm25_conc, pm5_index = get_pm25_interval_and_concentration(pm25_average_measured_conc=pm25_average_measured_conc)
    pm25_aiq = calculate_aiq(pm25_index_initial, pm25_index_final, pm25_conc_initial, pm25_conc_final,
                             pm25_avg_measured_pollution)
    return {'aiq': pm25_aiq, 'index_color': pm25_index_color, 'index_classification': pm25_index_classification}
