import wfdb
import numpy as np
np.set_printoptions(threshold=np.inf)
signals, fields = wfdb.rdsamp(r"ecg_data/ecg_data",channels=[0],sampto=175000)
wfdb.plot_items(signal=signals,fs=fields['fs'], title='ECG')
# Display it with the additional information
# display((signals, fields))
print(signals.tobytes(order='C'))
print(fields)
with open("dump.txt", 'wb') as f:
    f.write(signals.tobytes(order='C'))
    
