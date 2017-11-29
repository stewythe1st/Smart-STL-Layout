#!/bin/bash

cd matlab
matlab -r "run best_fitnesses.m; run best_fitness_vs_evals.m; exit();"
