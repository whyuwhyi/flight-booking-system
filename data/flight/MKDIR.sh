#!/usr/bin/fish

# 生成航班号列表
for i in (seq 0 199)
    set flight_number (printf "YY%04d" $i)

    # 创建文件夹和文件
    mkdir -p $flight_number
    for date in 2024-11-20 2024-11-21 2024-11-22 2024-11-23 2024-11-24
        touch $flight_number/$date.txt
    end
end
