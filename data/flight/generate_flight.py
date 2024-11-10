#!/usr/bin/env python3

import random
from datetime import datetime, timedelta

def read_models(model_file):
    models = []
    with open(model_file, 'r', encoding='utf-8') as f:
        lines = [line.strip() for line in f if line.strip()]
        num_models = int(lines[0])
        i = 1
        while i < len(lines):
            model_name = lines[i]
            cabin1 = [int(x) for x in lines[i+1].split()]
            cabin2 = [int(x) for x in lines[i+2].split()]
            cabin3 = [int(x) for x in lines[i+3].split()]
            models.append({
                'name': model_name,
                'first_class': cabin1,
                'business_class': cabin2,
                'economy_class': cabin3
            })
            i += 4
    return models

def read_airports(airport_file):
    airports = {}
    with open(airport_file, 'r', encoding='utf-8') as f:
        lines = [line.strip() for line in f if line.strip()]
        num_airports = int(lines[0])
        i = 1
        while i < len(lines):
            airport_name = lines[i]
            country = lines[i+1]
            city = lines[i+2]
            coords = lines[i+3]
            airports[airport_name] = {
                'country': country,
                'city': city,
                'coords': coords
            }
            i += 4
    return airports

def read_routes(route_file):
    routes = []
    with open(route_file, 'r', encoding='utf-8') as f:
        lines = [line.strip() for line in f if line.strip()]
        num_routes = int(lines[0])
        i = 1
        while i < len(lines):
            route_name = lines[i]
            dep_airport = lines[i+1]
            arr_airport = lines[i+2]
            distance = float(lines[i+3])
            waypoint_count = int(lines[i+4])
            waypoints = []
            for j in range(waypoint_count):
                waypoints.append(lines[i+5+j])
            i += 5 + waypoint_count
            routes.append({
                'name': route_name,
                'departure_airport': dep_airport,
                'arrival_airport': arr_airport,
                'distance': distance,
                'waypoints': waypoints
            })
    return routes

def generate_random_time():
    hours = random.randint(0,23)
    minutes = random.randint(0,59)
    return "{:02d}:{:02d}:00".format(hours, minutes)

def generate_random_duration():
    hours = random.randint(1,10)
    minutes = random.randint(0,59)
    return "{:02d}:{:02d}:00".format(hours, minutes)

def generate_initial_price():
    # 将头等舱初始价格范围提高到 20000 - 50000
    return random.randint(20000, 50000)

def generate_business_class_price(first_class_price):
    # 商务舱价格为头等舱的 50% - 70%
    return int(first_class_price * random.uniform(0.5, 0.7))

def generate_economy_class_price(first_class_price):
    # 经济舱价格为头等舱的 20% - 30%
    return int(first_class_price * random.uniform(0.2, 0.3))

def generate_daily_price(initial_price):
    # 每日价格比初始价格低，折扣范围为 5% - 20%
    discount = random.uniform(0.05, 0.2)
    return int(initial_price * (1 - discount))

def main():
    models = read_models("../airplanemodel/models.txt")
    airports = read_airports("../airport/airports.txt")
    routes = read_routes("../airline/airlines.txt")

    flight_counter = 0
    flight_prefix = "YY"
    total_flights = 200

    while flight_counter < total_flights:
        flight_number = "{}{:04d}".format(flight_prefix, flight_counter)

        # 随机选择一条航线
        selected_route = random.choice(routes)
        departure_airport = selected_route['departure_airport']
        arrival_airport = selected_route['arrival_airport']
        route_name = selected_route['name']

        # 随机交换出发和到达机场
        if random.choice([True, False]):
            departure_airport, arrival_airport = arrival_airport, departure_airport

        # 获取机场详情
        dep_details = airports.get(departure_airport)
        arr_details = airports.get(arrival_airport)

        if not dep_details:
            print(f"未找到机场信息: {departure_airport}")
        
        if not arr_details:
            print(f"未找到机场信息: {arrival_airport}")

        if not dep_details or not arr_details:
            # 如果未找到机场信息，跳过该航班
            flight_counter += 1
            continue

        # 随机选择机型
        selected_model = random.choice(models)
        first_class_config = selected_model['first_class']
        business_class_config = selected_model['business_class']
        economy_class_config = selected_model['economy_class']

        first_class_capacity = first_class_config[0] * first_class_config[1]
        business_class_capacity = business_class_config[0] * business_class_config[1]
        economy_class_capacity = economy_class_config[0] * economy_class_config[1]

        # 生成随机时间和价格
        departure_time = generate_random_time()
        duration_time = generate_random_duration()
        first_class_price = generate_initial_price()
        business_class_price = generate_business_class_price(first_class_price)
        economy_class_price = generate_economy_class_price(first_class_price)

        # 输出航班信息
        print(flight_number)
        print("YY航空")
        print(selected_model['name'])
        print(departure_airport)
        print(dep_details['country'])
        print(dep_details['city'])
        print(dep_details['coords'])
        print(arrival_airport)
        print(arr_details['country'])
        print(arr_details['city'])
        print(arr_details['coords'])
        print(route_name)
        print(f"{departure_time} {duration_time}")
        print(f"{first_class_config[0]} {first_class_config[1]}")
        print(f"{business_class_config[0]} {business_class_config[1]}")
        print(f"{economy_class_config[0]} {economy_class_config[1]}")
        print(f"{first_class_price} {business_class_price} {economy_class_price}")
        print("5")

        # 输出每日价格信息
        date = datetime.strptime("2024-11-20", "%Y-%m-%d")
        for _ in range(5):
            day_first_price = generate_daily_price(first_class_price)
            day_biz_price = generate_daily_price(business_class_price)
            day_eco_price = generate_daily_price(economy_class_price)
            print(f"{day_first_price} {day_biz_price} {day_eco_price}")
            print(f"{first_class_capacity} {business_class_capacity} {economy_class_capacity}")
            print(date.strftime("%Y-%m-%d"))
            date += timedelta(days=1)
        print()

        flight_counter += 1

if __name__ == "__main__":
    main()
