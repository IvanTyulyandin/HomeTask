import scipy.stats as stats
import numpy as np
import math


def get_sample_mean(data: list) -> float:
    return sum(data) / len(data)


def get_deviation(data: list) -> float:
    sqr_mean_diff: float = 0.0
    mean: float = get_sample_mean(data)
    data_size: int = len(data)
    for elem in data:
        sqr_mean_diff += math.pow(elem - mean, 2)
    return math.sqrt((1 / (data_size - 1)) * sqr_mean_diff)


def get_t_statistics_and_p_value(data: list, expected_mean: float) -> (float, float):
    mean: float = get_sample_mean(data)
    deviation: float = get_deviation(data)
    data_size: int = len(data)
    t_stat: float = (mean - expected_mean) / (deviation / math.sqrt(data_size))
    p_value = stats.t.sf(abs(t_stat), data_size - 1) * 2
    return t_stat, p_value


def main():
    data = [14, 14, 16, 13, 12, 17, 15, 14, 15, 13, 15, 14, 999999, 99999999]

    # 1-sample t-test
    # H0: mean = 15
    # H1: mean != 15
    supposed_mean = 15
    t_stat, p_value = stats.ttest_1samp(a=data, popmean=supposed_mean)
    print('T-statistic is', t_stat)
    print('p-value is', p_value)
    print(get_t_statistics_and_p_value(data, 15))

    # Comparisons of means with 2-sample t-test

    # fix random
    np.random.seed(12345678)

    print('\nSame distribution, p-value')
    rvs1 = stats.norm.rvs(loc=5, scale=10, size=500)
    rvs2 = stats.norm.rvs(loc=5, scale=10, size=500)
    print(stats.ttest_ind(rvs1, rvs2)[1])

    print('\nDifferent variance')
    rvs3 = stats.norm.rvs(loc=5, scale=20, size=500)
    print('False one:', stats.ttest_ind(rvs1, rvs3)[1])
    print('Correct is', stats.ttest_ind(rvs1, rvs3, equal_var=False)[1])

    print('\nDifferent variance and size')
    rvs4 = stats.norm.rvs(loc=5, scale=20, size=100)
    print('False one:', stats.ttest_ind(rvs1, rvs4)[1])
    print('Correct is', stats.ttest_ind(rvs1, rvs4, equal_var=False)[1])

    print('\nDifferent means, variance and size')
    rvs5 = stats.norm.rvs(loc=8, scale=20, size=100)
    print('False one:', stats.ttest_ind(rvs1, rvs5)[1])
    print('Correct is', stats.ttest_ind(rvs1, rvs5, equal_var=False)[1])


if __name__ == "__main__":
    main()
