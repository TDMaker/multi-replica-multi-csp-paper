# data_owner

# 审计步骤
1. 在 csp1 和 csp2 内分别进行 `make store`；
2. 执行 csp2 中的 `make challenge`;
3. 执行 csp1 中的 `make proof_gen1`;
4. 执行 csp2 中的 `make proof_gen2`;
5. 执行 csp1 中的 `make proof_verify1`;
6. 执行 csp2 中的 `make proof_verify2`;
