
Analyzing model 
C:/Users/l/STM32Cube/Repository/Packs/STMicroelectronics/X-CUBE-AI/10.2.0/Utilities/windows/stedgeai.exe analyze --target stm32f4 --name network -m C:/Users/l/Desktop/mnist_model_98.76_91.58.onnx --compression none --verbosity 1 --workspace C:/Users/l/AppData/Local/Temp/mxAI_workspace150241402591004647088914857464154 --output C:/Users/l/.stm32cubemx/network_output 
ST Edge AI Core v2.2.0-20266 2adc00962 
Creating c (debug) info json file C:\Users\l\.stm32cubemx\network_output\network_c_info.json 
  
 Exec/report summary (analyze) 
 ------------------------------------------------------------------------------------------------------- 
 model file         :   C:\Users\l\Desktop\mnist_model_98.76_91.58.onnx                                  
 type               :   onnx                                                                             
 c_name             :   network                                                                          
 compression        :   none                                                                             
 options            :   allocate-inputs, allocate-outputs                                                
 optimization       :   balanced                                                                         
 target/series      :   stm32f4                                                                          
 workspace dir      :   C:\Users\l\AppData\Local\Temp\mxAI_workspace150241402591004647088914857464154    
 output dir         :   C:\Users\l\.stm32cubemx\network_output                                           
 model_fmt          :   float                                                                            
 model_name         :   mnist_model_98_76_91_58                                                          
 model_hash         :   0x37bdfffe44b593a214c1f69c58a0424a                                               
 params #           :   211,347 items (825.57 KiB)                                                       
 ------------------------------------------------------------------------------------------------------- 
 input 1/1          :   'input', f32(1x28x28x1), 3.06 KBytes, activations                                
 output 1/1         :   'output', f32(1x10), 40 Bytes, activations                                       
 macc               :   418,195                                                                          
 weights (ro)       :   845,552 B (825.73 KiB) (1 segment) / +164(+0.0%) vs float model                  
 activations (rw)   :   19,268 B (18.82 KiB) (1 segment) *                                               
 ram (total)        :   19,268 B (18.82 KiB) = 19,268 + 0 + 0                                            
 ------------------------------------------------------------------------------------------------------- 
 (*) 'input'/'output' buffers are allocated in the activations buffer 
Computing AI RT data/code size (target=stm32f4).. 
 Model name - mnist_model_98_76_91_58 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 m_id   layer (original)                        oshape                 param/size             macc                     connected to 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 14     fc1_weight (Gemm)                       [b:128,c:1568]         200,704/802,816 
        fc1_bias (Gemm)                         [b:128]                128/512 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 16     fc2_weight (Gemm)                       [b:64,c:128]           8,192/32,768 
        fc2_bias (Gemm)                         [b:64]                 64/256 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 18     fc3_weight (Gemm)                       [b:10,c:64]            640/2,560 
        fc3_bias (Gemm)                         [b:10]                 10/40 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 0      input ()                                [b:1,h:28,w:28,c:1] 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 1      _conv1_depthwise_Conv_output_0 (Conv)   [b:1,h:28,w:28,c:1]    9/36                  7,056                            input 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 2      _conv1_pointwise_Conv_output_0 (Conv)   [b:1,h:28,w:28,c:16]   32/128               12,560   _conv1_depthwise_Conv_output_0 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 3      _Relu_output_0 (Relu)                   [b:1,h:28,w:28,c:16]                        12,544   _conv1_pointwise_Conv_output_0 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 4      _pool_MaxPool_output_0 (MaxPool)        [b:1,h:14,w:14,c:16]                        12,544                   _Relu_output_0 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 5      _conv2_depthwise_Conv_output_0 (Conv)   [b:1,h:14,w:14,c:16]   144/576              28,224           _pool_MaxPool_output_0 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 6      _conv2_pointwise_Conv_output_0 (Conv)   [b:1,h:14,w:14,c:24]   408/1,632            75,288   _conv2_depthwise_Conv_output_0 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 7      _Relu_1_output_0 (Relu)                 [b:1,h:14,w:14,c:24]                         4,704   _conv2_pointwise_Conv_output_0 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 8      _pool_1_MaxPool_output_0 (MaxPool)      [b:1,h:7,w:7,c:24]                           4,704                 _Relu_1_output_0 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 9      _conv3_depthwise_Conv_output_0 (Conv)   [b:1,h:7,w:7,c:24]     216/864              10,584         _pool_1_MaxPool_output_0 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 10     _conv3_pointwise_Conv_output_0 (Conv)   [b:1,h:7,w:7,c:32]     800/3,200            37,664   _conv3_depthwise_Conv_output_0 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 11     _Relu_2_output_0 (Relu)                 [b:1,h:7,w:7,c:32]                           1,568   _conv3_pointwise_Conv_output_0 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 13     _Reshape_output_0 (Reshape)             [b:1,c:1568]                                                       _Relu_2_output_0 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 14     _fc1_Gemm_output_0 (Gemm)               [b:1,c:128]                                200,832                _Reshape_output_0 
                                                                                                                         fc1_weight 
                                                                                                                           fc1_bias 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 15     _Relu_3_output_0 (Relu)                 [b:1,c:128]                                    128               _fc1_Gemm_output_0 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 16     _fc2_Gemm_output_0 (Gemm)               [b:1,c:64]                                   8,256                 _Relu_3_output_0 
                                                                                                                         fc2_weight 
                                                                                                                           fc2_bias 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 17     _Relu_4_output_0 (Relu)                 [b:1,c:64]                                      64               _fc2_Gemm_output_0 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 18     output (Gemm)                           [b:1,c:10]                                     650                 _Relu_4_output_0 
                                                                                                                         fc3_weight 
                                                                                                                           fc3_bias 
 ------ --------------------------------------- ---------------------- ----------------- --------- -------------------------------- 
 model: macc=417,370 weights=845,388 activations=-- io=-- 
 Number of operations per c-layer 
 ------- ------ ----------------------------------------- --------- -------------- 
 c_id    m_id   name (type)                                     #op           type 
 ------- ------ ----------------------------------------- --------- -------------- 
 0       1      _conv1_depthwise_Conv_output_0 (Conv2D)       7,057   smul_f32_f32 
 1       4      _conv1_pointwise_Conv_output_0 (Conv2D)      37,648   smul_f32_f32 
 2       5      _conv2_depthwise_Conv_output_0 (Conv2D)      28,240   smul_f32_f32 
 3       8      _conv2_pointwise_Conv_output_0 (Conv2D)      84,696   smul_f32_f32 
 4       9      _conv3_depthwise_Conv_output_0 (Conv2D)      10,608   smul_f32_f32 
 5       10     _conv3_pointwise_Conv_output_0 (Conv2D)      37,664   smul_f32_f32 
 6       11     _Relu_2_output_0 (Nonlinearity)               1,568     op_f32_f32 
 7       13     _Reshape_output_0_to_chlast (Transpose)         784   smul_f32_f32 
 8       14     _fc1_Gemm_output_0 (Dense)                  200,832   smul_f32_f32 
 9       15     _Relu_3_output_0 (Nonlinearity)                 128     op_f32_f32 
 10      16     _fc2_Gemm_output_0 (Dense)                    8,256   smul_f32_f32 
 11      17     _Relu_4_output_0 (Nonlinearity)                  64     op_f32_f32 
 12      18     output (Dense)                                  650   smul_f32_f32 
 ------- ------ ----------------------------------------- --------- -------------- 
 total                                                      418,195 
 Number of operation types 
 ---------------- --------- ----------- 
 operation type           #           % 
 ---------------- --------- ----------- 
 smul_f32_f32       416,435       99.6% 
 op_f32_f32           1,760        0.4% 
 Complexity report (model) 
 ------ -------------------------------- ------------------------- ------------------------- ------ 
 m_id   name                             c_macc                    c_rom                     c_id 
 ------ -------------------------------- ------------------------- ------------------------- ------ 
 14     fc1_weight                       ||||||||||||||||  48.0%   ||||||||||||||||  95.0%   [8] 
 16     fc2_weight                       |                  2.0%   |                  3.9%   [10] 
 18     fc3_weight                       |                  0.2%   |                  0.3%   [12] 
 1      _conv1_depthwise_Conv_output_0   |                  1.7%   |                  0.0%   [0] 
 4      _pool_MaxPool_output_0           |||                9.0%   |                  0.0%   [1] 
 5      _conv2_depthwise_Conv_output_0   |||                6.8%   |                  0.1%   [2] 
 8      _pool_1_MaxPool_output_0         |||||||           20.3%   |                  0.2%   [3] 
 9      _conv3_depthwise_Conv_output_0   |                  2.5%   |                  0.1%   [4] 
 10     _conv3_pointwise_Conv_output_0   |||                9.0%   |                  0.4%   [5] 
 11     _Relu_2_output_0                 |                  0.4%   |                  0.0%   [6] 
 13     _Reshape_output_0                |                  0.2%   |                  0.0%   [7] 
 15     _Relu_3_output_0                 |                  0.0%   |                  0.0%   [9] 
 17     _Relu_4_output_0                 |                  0.0%   |                  0.0%   [11] 
 ------ -------------------------------- ------------------------- ------------------------- ------ 
 macc=418,195 weights=845,552 act=19,268 ram_io=0 
 Requested memory size by section - "stm32f4" target 
 ------------------------------ -------- --------- ------- -------- 
 module                             text    rodata    data      bss 
 ------------------------------ -------- --------- ------- -------- 
 NetworkRuntime1020_CM4_GCC.a     14,372         0       0        0 
 network.o                         1,004       104   4,776      244 
 network_data.o                       48        16      88        0 
 lib (toolchain)*                      0         0       0        0 
 ------------------------------ -------- --------- ------- -------- 
 RT total**                       15,424       120   4,864      244 
 ------------------------------ -------- --------- ------- -------- 
 weights                               0   845,552       0        0 
 activations                           0         0       0   19,268 
 io                                    0         0       0        0 
 ------------------------------ -------- --------- ------- -------- 
 TOTAL                            15,424   845,672   4,864   19,512 
 ------------------------------ -------- --------- ------- -------- 
 *  toolchain objects (libm/libgcc*) 
 ** RT AI runtime objects (kernels+infrastructure) 
  Summary - "stm32f4" target 
  -------------------------------------------------- 
               FLASH (ro)     %*   RAM (rw)       % 
  -------------------------------------------------- 
  RT total         20,408   2.4%      5,108   21.0% 
  -------------------------------------------------- 
  TOTAL           865,960            24,376 
  -------------------------------------------------- 
  *  rt/total 
Creating txt report file C:\Users\l\.stm32cubemx\network_output\network_analyze_report.txt 
elapsed time (analyze): 18.641s 
Model file:      mnist_model_98.76_91.58.onnx 
Total Flash:     865960 B (845.66 KiB) 
    Weights:     845552 B (825.73 KiB) 
    Library:     20408 B (19.93 KiB) 
Total Ram:       24376 B (23.80 KiB) 
    Activations: 19268 B (18.82 KiB) 
    Library:     5108 B (4.99 KiB) 
    Input:       3136 B (3.06 KiB included in Activations) 
    Output:      40 B (included in Activations) 
Done 
Analyze complete on AI model



Starting AI validation on desktop with random data...
  
C:/Users/l/STM32Cube/Repository/Packs/STMicroelectronics/X-CUBE-AI/10.2.0/Utilities/windows/stedgeai.exe validate --target stm32f4 --name network -m C:/Users/l/Desktop/mnist_model_98.76_91.58.onnx --compression none --verbosity 1 --workspace C:/Users/l/AppData/Local/Temp/mxAI_workspace151095843048001862637438405954232 --output C:/Users/l/.stm32cubemx/network_output 
ST Edge AI Core v2.2.0-20266 2adc00962 
Setting validation data... 
 generating random data, size=10, seed=42, range=(0, 1) 
   I[1]: (10, 28, 28, 1)/float32, min/max=[0.000012, 0.999718], mean/std=[0.495359, 0.288935] 
 no output/reference samples are provided 
Creating c (debug) info json file C:\Users\l\.stm32cubemx\network_output\network_c_info.json 
Copying the AI runtime files to the user workspace: C:\Users\l\AppData\Local\Temp\mxAI_workspace151095843048001862637438405954232\inspector_network\workspace 
 from: C:\Users\l\STM32Cube\Repository\Packs\STMicroelectronics\X-CUBE-AI\10.2.0\Utilities\windows\targets\common\EmbedNets\tools\inspector\workspace 
  
 Exec/report summary (validate) 
 ------------------------------------------------------------------------------------------------------- 
 model file         :   C:\Users\l\Desktop\mnist_model_98.76_91.58.onnx                                  
 type               :   onnx                                                                             
 c_name             :   network                                                                          
 compression        :   none                                                                             
 options            :   allocate-inputs, allocate-outputs                                                
 optimization       :   balanced                                                                         
 target/series      :   stm32f4                                                                          
 workspace dir      :   C:\Users\l\AppData\Local\Temp\mxAI_workspace151095843048001862637438405954232    
 output dir         :   C:\Users\l\.stm32cubemx\network_output                                           
 model_fmt          :   float                                                                            
 model_name         :   mnist_model_98_76_91_58                                                          
 model_hash         :   0x37bdfffe44b593a214c1f69c58a0424a                                               
 params #           :   211,347 items (825.57 KiB)                                                       
 ------------------------------------------------------------------------------------------------------- 
 input 1/1          :   'input', f32(1x28x28x1), 3.06 KBytes, activations                                
 output 1/1         :   'output', f32(1x10), 40 Bytes, activations                                       
 macc               :   418,195                                                                          
 weights (ro)       :   845,552 B (825.73 KiB) (1 segment) / +164(+0.0%) vs float model                  
 activations (rw)   :   19,268 B (18.82 KiB) (1 segment) *                                               
 ram (total)        :   19,268 B (18.82 KiB) = 19,268 + 0 + 0                                            
 ------------------------------------------------------------------------------------------------------- 
 (*) 'input'/'output' buffers are allocated in the activations buffer 
Running the ONNX model... 
Running the ST.AI c-model (AI RUNNER)...(name=network, mode=HOST) 
 DLL Driver v2.0 - Direct Python binding (C:\Users\l\AppData\Local\Temp\mxAI_workspace151095843048001862637438405954232\inspector_network\workspace\lib\libai_network.dll) ['network'] 
  Summary 'network' - ['network'] 
  -------------------------------------------------------------------------------------------- 
  I[1/1] 'input_1'    :   f32[1,28,28,1], 3136 Bytes, in activations buffer 
  O[1/1] 'output_1'   :   f32[1,1,1,10], 40 Bytes, in activations buffer 
  n_nodes             :   13 
  activations         :   19268 
  weights             :   845552 
  macc                :   418195 
  hash                :   0x37bdfffe44b593a214c1f69c58a0424a 
  compile_datetime    :   Jan 14 2026 16:04:38 
  -------------------------------------------------------------------------------------------- 
  protocol            :   DLL Driver v2.0 - Direct Python binding 
  tools               :   ST.AI (legacy api) v2.2.0 
  runtime lib         :   v10.2.0 
  capabilities        :   IO_ONLY, PER_LAYER, PER_LAYER_WITH_DATA 
  device.desc         :   AMD64, AMD64 Family 25 Model 97 Stepping 2, AuthenticAMD, Windows 
  -------------------------------------------------------------------------------------------- 
 NOTE: The duration and execution time per layer are just indications. They depend on the host machine's workload. 
  ST.AI Profiling results v2.0 - "network" 
  ------------------------------------------------------------ 
  nb sample(s)   :   10 
  duration       :   0.309 ms by sample (0.284/0.375/0.031) 
  macc           :   418195 
  ------------------------------------------------------------ 
  Statistic per tensor 
  ----------------------------------------------------------------------------------- 
  tensor   #    type[shape]:size           min     max     mean      std  name 
  ----------------------------------------------------------------------------------- 
  I.0      10   f32[1,28,28,1]:3136      0.000   1.000    0.495    0.289  input_1 
  O.0      10   f32[1,1,1,10]:40        -5.706   6.658   -0.442    2.743  output_1 
  ----------------------------------------------------------------------------------- 
Saving validation data... 
 output directory: C:\Users\l\.stm32cubemx\network_output 
 creating C:\Users\l\.stm32cubemx\network_output\network_val_io.npz 
 m_outputs_1: (10, 10)/float64, min/max=[-5.705760, 6.657891], mean/std=[-0.442338, 2.743048], output 
 c_outputs_1: (10, 1, 1, 10)/float32, min/max=[-5.705760, 6.657890], mean/std=[-0.442338, 2.743047], output 
Computing the metrics... 
 Cross accuracy report #1 (reference vs C-model) 
 ---------------------------------------------------------------------------------------------------- 
 notes: - data type is different: r/float64 instead p/float32 
        - ACC metric is not computed ("--classifier" option can be used to force it) 
        - the output of the reference model is used as ground truth/reference value 
        - 10 samples (10 items per sample) 
  acc=n.a. rmse=0.000000533 mae=0.000000381 l2r=0.000000192 mean=-0.000000 std=0.000001 nse=1.000000 cos=1.000000 
 Evaluation report (summary) 
 ------------------------------------------------------------------------------------------------------------------------------------------------- 
 Output       acc    rmse          mae           l2r           mean        std        nse        cos        tensor 
 ------------------------------------------------------------------------------------------------------------------------------------------------- 
 X-cross #1   n.a.   0.000000533   0.000000381   0.000000192   -0.000000   0.000001   1.000000   1.000000   'output', 10 x f32(1x10), m_id=[18] 
 ------------------------------------------------------------------------------------------------------------------------------------------------- 
  acc  : Accuracy (class, axis=-1) 
  rmse : Root Mean Squared Error 
  mae  : Mean Absolute Error 
  l2r  : L2 relative error 
  mean : Mean error 
  std  : Standard deviation error 
  nse  : Nash-Sutcliffe efficiency criteria, bigger is better, best=1, range=(-inf, 1] 
  cos  : COsine Similarity, bigger is better, best=1, range=(0, 1] 
Creating txt report file C:\Users\l\.stm32cubemx\network_output\network_validate_report.txt 
elapsed time (validate): 11.753s 
Validation ended


Starting AI validation on target with random data... 
C:/Users/l/STM32Cube/Repository/Packs/STMicroelectronics/X-CUBE-AI/10.2.0/Utilities/windows/stedgeai.exe validate --target stm32f4 --name network -m C:/Users/l/Desktop/mnist_model_98.76_91.58.onnx --compression none --verbosity 1 --workspace C:/Users/l/AppData/Local/Temp/mxAI_workspace275557114620001782796457663227146 --output C:/Users/l/.stm32cubemx/network_output --mode target --desc serial:COM6:115200 
ST Edge AI Core v2.2.0-20266 2adc00962 
Setting validation data... 
 generating random data, size=10, seed=42, range=(0, 1) 
   I[1]: (10, 28, 28, 1)/float32, min/max=[0.000012, 0.999718], mean/std=[0.495359, 0.288935] 
 no output/reference samples are provided 
Creating c (debug) info json file C:\Users\l\.stm32cubemx\network_output\network_c_info.json 
  
 Exec/report summary (validate) 
 ------------------------------------------------------------------------------------------------------- 
 model file         :   C:\Users\l\Desktop\mnist_model_98.76_91.58.onnx                                  
 type               :   onnx                                                                             
 c_name             :   network                                                                          
 compression        :   none                                                                             
 options            :   allocate-inputs, allocate-outputs                                                
 optimization       :   balanced                                                                         
 target/series      :   stm32f4                                                                          
 workspace dir      :   C:\Users\l\AppData\Local\Temp\mxAI_workspace275557114620001782796457663227146    
 output dir         :   C:\Users\l\.stm32cubemx\network_output                                           
 model_fmt          :   float                                                                            
 model_name         :   mnist_model_98_76_91_58                                                          
 model_hash         :   0x37bdfffe44b593a214c1f69c58a0424a                                               
 params #           :   211,347 items (825.57 KiB)                                                       
 ------------------------------------------------------------------------------------------------------- 
 input 1/1          :   'input', f32(1x28x28x1), 3.06 KBytes, activations                                
 output 1/1         :   'output', f32(1x10), 40 Bytes, activations                                       
 macc               :   418,195                                                                          
 weights (ro)       :   845,552 B (825.73 KiB) (1 segment) / +164(+0.0%) vs float model                  
 activations (rw)   :   19,268 B (18.82 KiB) (1 segment) *                                               
 ram (total)        :   19,268 B (18.82 KiB) = 19,268 + 0 + 0                                            
 ------------------------------------------------------------------------------------------------------- 
 (*) 'input'/'output' buffers are allocated in the activations buffer 
Running the ONNX model... 
Running the ST.AI c-model (AI RUNNER)...(name=network, mode=TARGET) 
E200(ValidationError): TARGET: Unable to bind the ST.AI runtime with "network" c-model: [] 
 E801(HwIOError): Invalid firmware - COM6:115200 
Validation ended