# 首先定义航班号列表
set flight_numbers "HU5204" "MU8201" "CZ3308" "CA5193" "MU6028" "CA2090"

# 创建文件夹和文件
for flight in $flight_numbers
    mkdir -p $flight
    for date in 2024-11-21 2024-11-22 2024-11-23 2024-11-24 2024-11-25
        touch $flight/$date.txt
    end
end

