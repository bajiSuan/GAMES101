
I've finished task ...

- [x] [5 points] 提交格式正确，包含所有需要的文件；代码可以在虚拟机下正确编译运行。
- [x] [20 points] 包围盒求交：正确实现光线与包围盒求交函数。
- [x] [15 points] BVH 查找：正确实现 BVH 加速的光线与场景求交。
- [ ] [加分项 20 points] SAH 查找：自学 SAH(Surface Area Heuristic) , 正确实现 SAH 加速，并且提交结果图片，并在 README.md 中说明 SVH 的实现方法，并对比 BVH、SVH 的时间开销。(可参考 http://15462.courses.cs.cmu.edu/fall2015/lecture/acceleration/slide_024 也可以查找其他资料)



### 1. Ray Intersection with Axis-Aligned Box

- Recall: a box (3D) = three pairs of infinitely large slabs
-  Key ideas
  - The ray enters the box only when it enters all pairs of slabs
  - The ray exits the box as long as it exits any pair of slabs
- For each pair, calculate the tmin and tmax (negative is fine)
- For the 3D box, tenter = max{tmin}, texit = min{tmax}
- If tenter < texit, we know the ray stays a while in the box
  (so they must intersect!) (not done yet, see the next slide)
- However, ray is not a line
  - Should check whether t is negative for physical correctness!
- What if texit < 0?
  - The box is “behind” the ray — no intersection!
- What if texit >= 0 and tenter < 0?
  - The ray’s origin is inside the box — have intersection!
- In summary, ray and AABB intersect iff
  - tenter < texit && texit >= 0

### 2. BVH Traversal

```c++
Intersect(Ray ray, BVH node) {
	if (ray misses node.bbox) return;
    
	if (node is a leaf node)
		test intersection with all objs;
		return closest intersection;
    
	hit1 = Intersect(ray, node.child1);
	hit2 = Intersect(ray, node.child2);
    
	return the closer of hit1, hit2;
}
```

