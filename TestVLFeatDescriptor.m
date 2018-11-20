I = zeros(256,240);
I(128,:)=255;
[frames, descriptors] = vl_sift(single(I),'frames',FC','floatdescriptors','octaves',1,'firstoctave',0,'verbose','verbose','verbose');



DisplayDescriptorGradient('./grads.txt')