# Artiface description
## 1. Artifact Identification
### Article's title

CIA: A Collaborative Integrity Auditing Scheme for Cloud Data with Multi-Replica on Multi-Cloud Storage Providers

### Authors
- Tengfei Li, ltf15@mails.jlu.edu.cn
- Jiangfeng Chu, chujf@jlu.edu.cn,
- Liang Hu (Corresponding author), hul@jlu.edu.cn

All authors are with College of Computer Science and Technology, Jilin University, China. 

### Abstrct
In this paper we propose a new model for remote data integrity auditing: CIA (Collaborative Integrity Auditing).
In addition to the reduction in computational overhead, the proposed scheme provides unprecedented support for free data blocking.
The proposed scheme employs only hash functions in the calculation, which has a negligible computational overhead compared to the traditional bilinear pairing-based schemes.
The proposed scheme provides high efficiency and flexibility with security assurance, and can be used as a lightweight alternative to traditional remote data integrity auditing schemes in multi-replica multi-CSP scenarios.

## 2. Dependencies and Requirements
### Hardware
We performed the algorithms on a laptop with the processor of Intel(R) Core(TM) i7-6500U CPU @ 2.50GHz × 4 and 4 GB of RAM. Any general-purpose computing device works.
### Operating Systems
The operating system we used is Ubuntu Desktop 22.04, Windows, Mac or other common systems are fine.
### Software Libraries
The library employed in our scheme is GMP, and PBC is required by the compared two schemes.
### Input Dataset
The data type doesn't matter, as long as it's a file.
## 3. Installation and Deployment Process
The software does not need to be installed and deployed, it just needs to generate an executable file based on the instructions in the makefile in each directory. The compilation process takes a negligible amount of time.

The installation of the dependent libraries [GMP](https://gmplib.org/) and [PBC](https://crypto.stanford.edu/pbc/) used can be done according to the instructions on the respective homepage.

## 4. Reproducibility of Experiments
*mdss* and *mur-dpr* are the implementations of two compared schemes, so we will not go into details here, but only present the implementation of our scheme *cia*.

The following commands are all executed in Linux shell such as Bash.
### Workflow
1. In the *csp1* and *csp2* directories, execute `make store` and `./a.out`, respectively.
2. In *csp2*, `make challenge` and `./a.out`.
3. In *csp1*, `make proof_gen1` and `./a.out`.
4. In *csp2*, `make proof_gen2` and `./a.out`.
5. In *csp1*, `make proof_verify1` and `./a.out`.
6. In *csp2*, `make proof_verify2` and `./a.out`.


### Estimation of execution time
As mentioned above, the algorithm consists of four steps, `store`, `challenge`, `proof_gen` and `proof_verify`. Depending on the performance of the hardware and the specified parameters, the time consumed by each algorithm varies.

Time overhead is the major comparison index in the experimental part of the paper. We have set up various settings in the paper to compare the difference in time overhead between the proposed and the compared schemes.
### Description of the results
Let's start with the parameters that need to be defined for the program, which are scattered throughout the files in the form of macro definitions.

- `BASE_PATH`: the base path of all the data.
- `SOURCED_FILE_NAME`: the name of the file to be outsourced.
- `BLK_PATH`: the path data blocks stored at CSPs.
- `CHAL_AMOUNT`: the number of blocks to be challenged.
- `SECURITY_LEVEL`: space for all integer operations, usually 160 bits
- `BLOCK_AMOUNT`: the total number of block the source file divided to.
- `DEFAULT_BLK_SIZE`: the default block size.

As mentioned above, the execution output of each algorithm file is the time consumed by this algorithm at the corresponding setting.
### Result relation
In the paper, the figures related to the experimental results are Figure 2 to Figure 7, and the figures associated with this scheme are Figs. 4 to 7.

**Fig.4**

`BLOCK_AMOUNT=5000`, `DEFAULT_BLK_SIZE=20` and `CHAL_AMOUNT=300`, 

Trend of the sum of time consumption of `proof_gen.c` and `proof_verify.c` with the growth of the number of replicas.

**Fig.5**

`BLOCK_AMOUNT=5000`, `DEFAULT_BLK_SIZE=20`

Trend of the sum of time consumption of `proof_gen.c` and `proof_verify.c` with the growth of the number of `CHAL_AMOUNT`. 

**Fig.6**

`DEFAULT_BLK_SIZE=4*1024`, `BLOCK_AMOUNT=5000`

Trend of the sum of time consumption of `proof_gen.c` with the grouwth of the number of `CHAL_AMOUNT`.

**Fig.7**

`CHAL_AMOUNT=500`
Trend of the sum of time consumption of `proof_gen.c` with the grouwth of the number of `DEFAULT_BLK_SIZE`.