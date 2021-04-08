
### Brute Force Implementation ###
class DisparityMap():
    def __init__(self, numDisparities, blockSize):
        self.numDisparities = numDisparities
        self.blockSize = blockSize
    
    def Hamming_distance(self, a, b):
        return sum(a[i]!=b[i] for i in range(len(a)))
        
    def CT(self, imgL, imgR):
        imgL = imgL.astype(np.float)
        imgR = imgR.astype(np.float)
        l, w = imgL.shape
        disparityMap = np.zeros((l, w))
        mid = int(self.blockSize/2)
        
        for i in range(mid, l-mid):
            for j in range(mid, w-mid):
                
                #Finding csl value for right block
                csr = ''
                for ii in range(i-mid, i+mid+1):
                    for jj in range(j-mid, j+mid+1):
                        if imgR[ii, jj]<imgR[i, j]:
                            csr += '0'
                        else:
                            csr += '1'
                
                #Comparing the csr_val with the left image and generating error
                errors = []
                for k in range(j, min(j+self.numDisparities, w-mid)):
                    #Finding csl value for right block
                    csl = ''
                    for ii in range(i-mid, i+mid+1):
                        for jj in range(k-mid, k+mid+1):
                            if imgL[ii, jj]<imgL[i, k]:
                                csl += '0'
                            else:
                                csl += '1'
                    errors.append(sum(csl[p]!=csr[p] for p in range(len(csl))))
                disparityMap[i, j] = np.argmin(errors)
        return disparityMap