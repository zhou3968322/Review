
## What's a r-ruling set?


输入:连通有向图G(V, E)，每个顶点的入度都是1。每个顶点的出度都是1。这样的图被称为环，因为它形成了一个有向circuit。设n = |V|。我们定义V的子集U是G的r-ruling set，如果:

1. U的两个顶点不相邻。
2. 对于V中的每个顶点v，存在一条从v到U中某个顶点的有向路径，该顶点的边长不超过r

r-ruling set问题:找到V的r-ruling set

### Assumptions about the input representation

顶点通过长度为n的数组中给出。数组的条目编号从0到n- 1。这些数字被表示为长度为ceil(logn)的二进制字符串——统治集问题。我们用0到ceil(logn) - 1之间的一个数字来指代这种表示的每个二进制符号(位)。最右边(最低有效)的位称为位号0，最左边的位称为位号ceil(logn) - 1。每个顶点都有一个指向环中下一个顶点的指针(表示其外向边)。为了简单起见，我们假设log n是一个整数。

下面是对log n ruling-set 问题的一个算法的口头描述。算法在后面给出。处理器i, 0 =< i <= n - 1，被分配给输入数组的第i项(为简单起见，第i项称为顶点i)。它将数字i附加到顶点i。因此，顶点i的当前“串行”编号，表示为SERIAL_0(i)，是i。接下来，我们为顶点i附加一个新的序列号，表示为serial_1 (i)，如下所示。设i2是i之后的顶点(即(i, i2)在e中)设j为“i和i2最右位的索引”处理器i将j赋值给SERIALs_1(i)。

**Example**:如果i 是...010101, i2是111101，那么i和i2 rightmost不同的bit位是3，因此SERIAL_1(i) = 3.

**Remark (due to B. Schieber)**: j可以通过常量的标准运算来计算，如下所示。在不丧失一般性的前提下，假设i>= i2(否则互换这两个数)。设h = i- i2, k = h - 1。(因此，h的第j位为1，比j要低的bit位为0,k的第j位为0，比j bit位为低的bit位为1;此外，h和k在比j高的bit为上一致).
计算l = h XOR k，其中XOR是异或运算。我们观察到l是j + 1的unary表示。所以剩下的就是把这个值从unary转换成binary，然后减去1。

接下来，可以看如果通过SERIAL_1的vector来找到一个logn -ruling set.

### Some Important Facts

**Fact1**: 对于所有的i,SERIAL_1(i)是一个在0- log(n)-1之间的数，并且只需要ceil(log(logn))来表示,为了尽量简单，我们假定 log logn是一个integer.

**Illustration**: 对于vertex i,SERIAL_0(i)最多 log(n) - 1 bit，所以SERIAL_1(i)是一个在0- log(n)-1 之间的数.

对于i1->i->i2，如果SERIAL_1(i) <= SERIAL_1(i2), SERIAL_1(i) <= SERIAL_1(i1)，那么SERIAL_1(i) 是一个局部最小值，同样有局部最大值的定义.

**FACT2**: 从G中任意vertex u去找到相对于SERIAL_1的local极值的最短path p: u->v1->v2...->vk->ve,p 的顶点数最多为 logn.

**Illustration**: 如果有一个path p到下一个local 的shortest path的顶点数为 logn + 1，那么 u->v1->v2 ...->ve必定构成一个单调序列. 否则假设：
- SERIAL_1(vk) >= SERIAL_1(ve)，那么SERIAL_1(vk) <= SERIAL_1(vk-1) <=  SERIAL_1(vk-2) <= ... <= SERIAL_1(u), logn + 1个数中必定有两个i->j相等.SERIAL_1(i) = SERIAL_1(j)，它形成的k->i->j，如果SERIAL_1(k) < SERIAL_1(i)，SERIAL_1(k) <= SERIAL_1(i), SERIAL_1(i) >= SERIAL_1(j), i就是局部最小值，那么u->i相对于u->ve是一条更加短的path.
  
观察到几个local 最小值（或者local最大值）可能形成一个 G中vertices的chain.对于r-ruling set定义中的要求1，我们是不能把所有的局部最小值都包括到选择的节点上，该算法利用vector SERIAL_1的交替属性来客服这个问题.

**The alternation property**: i->j, i,j均为vertex，如果SERIAL_0(i)第SERIAL_1(i) bit number 是 0（或者1），那么SERIAL_0(j)的SERIAL_1(i) bit number 是 1（或0）.(因为SERIAL_1(i)是SERIAL_0(i)和SERIAL_0(j) bit不同的 最右边的bit)

假设i1,i2,... 是G的一个chain，对于任意chain中的vertex i, SERIAL_1(i)都是一个局部最小值（或局部最大值），那么有:

**Fact3**: 这个chain中所有的vertex i的SERIAL_1(i)是相等的.(通过定义)

下面我们考虑chain中所有vertices的SERIAL_0，对应SERIAL_1(i1) 位置的bit number.

**Fact4**: 下面的bit序列是一个0,1交替的序列:

Bit Number SERIAL_1(i1) of SERIAL_0(i1), Bit Number SERIAL_1(i2) of SERIAL_0(i2), ..., Bit Number SERIAL_1(i_j) of SERIAL_0(i_j)

### deterministic coin tossing

现在我们可以理解为什么我们称我们的技术为确定性硬币了扔。我们把0和1和顶点联系起来，基于它们的原始序列号;这些序列号是确定设置的。
这种关联允许我们以不同的方式对待(显然)相似的顶点。最后，请注意抛硬币可以用于类似的目的：

我们回到算法。我们选择以下顶点子集。我们选择所有的顶点i是局部最小值并且满足以下两个条件的其中一个（Set1）:

1. i的邻居（只有两个邻居）都不是一个局部最小值.
2. i的SERIAL_1(i) bit number是1

如果一个vertex,它的邻居都没有被选择到Set1，并且它是一个局部最大值，我们就说它是available的(Set2)。

之后我们选择满足以下两个性质之一的所有available vertices (Set3)：
1. i的邻居都不是available的
2. i的SERIAL_1(i) bit number是1

选择的节点就形成了一个 logn-ruling set. 
两个条件中的（1）直接就能满足，因为不会同时选择到两个相邻的available vertice.
（2）也能被满足，因为Fact2，并且每个local极值要么被选择，要么是一个被选择节点的邻居.

**Illustration**:
Case1: 如果v是局部最小值
Case1.1:v的邻居i,j如果都不是局部最小值，那么v就在 Set1中.
Case1.2:v的邻居i,j 不可能满足Set1的条件1，如果i的SERIAL_1(i) bit number是1, i就被选择到Set1和Set3中,v就不是available的，但v是i的邻居，满足property.j的情况和i类似
Case1.3:如果i,j的SERIAL_1(i) bit number, SERIAL_1(j) bit number是0，那么由于v是局部最小值，所以v的SERIAL_1(v) Bit Number为0. v也是local max. 那么对于v而言，i,j都没有被选择到Set1，v就是available的，v 在Set2中，同样也在Set3中.
Case2: 如果v是局部最大值，同理.

下面是对应的算法（之后称之为basic step）

**Algorithms**:
```
for Processor i, 0<= i <= n - 1 pardo(perform in parallel)
    SERIAL0(i) := i
    SERIAL1(i) := "the minimal bit in which SERIAL0(i) differs from SERIAL0 of the following vertex"
    if SERIAL1(i) is a local minimum with respect to the two neighbors of i then
    then if either of the following is satisfied:
        (1) neither of the vertices adjacent to i is a local minum
        (2) bit number SERIAL1(i) of SERIAL0(i) is 1
        then select i
    if neither i nor any of its neighbors were selected and if SERIAL1(i) is a local maximu with respect the two neighbors of i
    then (** is availabele, and **) if either of the following is satisfied:
        (1) neither of the vetices adjacent to i is available
        (2) bit number SERIAL1(i) of SERIAL0(I) IS 1
        then select i.
```


**Theorem2.1**: 一个logn-ruling set能够使用n processors在O(1) time获取.

## The kth Application of the Basic Step

首先是将原始算法改为迭代的算法.

为了准备基本步骤的第k次应用的输入，我们从G中“删除”之前k-1 迭代选择的vertices，它们的neighbors,和相关的边.通过删除的方式可以简化graph.

对于kth application of the basic step,SERIAL(k-1, i)就相当于SERIAL_0,新的SERIAL(k,i)就相当于SERIAL_1的作用.

由于input graph中vertex的degree最多为2.如果vertex degree <=1 的话，要么是起点，要么是终点，要么是孤立点。如果vertex degree = 2，那么就只需要考虑application的basic step，也就是前面的步骤.（compute SERIAL(k, i)的过程其实也就是重新编号的过程.）

**Algorithm**:

```
for processor i, 0 =< i <= n - 1, pardo 
    if vertex i or one of its neighbors have been selected in a previous application of the basic step 
        then "delete" vertex i and the edges incident to it 
for processor i, 0 =< i <= n - 1, such that i is in the remaining graph pardo 
    case 1 deg(i)= 2 
        then compute SERIAL(k, i) 
        if the degree of each of it's two neighbors is 2
            then apply the basic step to i (i不是孤立点，)
    case 2 deg(i)= 0 
        then select i 
    case 3 deg(i)= 1 
        then if either of the following is satisfied 
            (1) the degree of i's neighbor is 2 
            (2) i's neighbor is its successor 
        then select i
```

下面的fact可以帮助阐述the basic step的第kth application.

**Fact5**: 如果i,j在kth application是相邻的,那么SERIAL(k-1, i) 不等于 SERIAL(k-1, j).

**Illustration**:对于k=1显然成立，i,j相邻，那么SERIAL_0(i) \ne SERIAL_0(j)
对于k>1,它也成立。如果SERIAL(k-1, i) = SERIAL(k-1, j)，那么在(k-1)st application中，它们一定是local min或者local max.那么根据ruling-set的选择过程可以推导出，每个local极值要么被选择或者是一个选择的neighbor.因此v一定已经被删除并且不可能在input graph中.


**Fact6**: 很容易推断出output graph由简单的每条路径最多包含log log... log n个顶点序列(包括k个log)(同样，为了简单起见，我们假设log log... log n列只产生整数。)

通过描述我们得到一个明显的结论:

1. 通过一系列的 log*n applications，我们可以删除graph中所有的节点.
2. selected的vertices形成了一个2-ruling set
3. 2-ruling set的基数至少是n/3.（由于max degree是2）

**Illustration**:在kth application的过程中,每个被deleted vertex v, 会使得它的prev degree - 1, 也就是变成0，或者1. 
- (1)prev是0，prev就被selected，那么prev->v, v就被prev 2-ruling.
- (2)prev是1，原始的G' pp->prev->v, pp一定有后继，所以pp一定被selected,v被pp 2-ruling.


如果我们的原始输入是n个顶点的有向路径，而不是一个ring，我们如上所述，通过应用基本步骤log*n次获得一个2-ruling set。为了得到一个log^(k) - ruling set，我们需要使用basic step k次。

也就得到如下:

**Theorem2.2**: A log^{k} n -ruling set可以使用n processors 在O(k) time获取。

**推论2.1**: 一个2-ruling set可以使用n processors在O(log*n) 时间获取.

## An Optimal 2-Ruling Set Algorithm


首先，我们使用上面的基本步骤找到一个log n-ruling set。下面,我们描述如何向log n-ruling set 添加更多vertex以生成2-ruling set。这些额外的vertex是使用每个顶点关联的SERIAL1 numbers来选择的，如下所示：

```
fori=0 to logn-1 do 
    for each vertex v for which SERIAL_1(v) = i pardo 
        if v is not in the ruling set and neither of the neighbors of v is in the ruling set 
        then add v to the ruling set
```

**Illustration**: 如果SERIAL_1(v) = i：
- 如果v和它的neighbors 都不在logn -ruling set中，那么对于v和它的neighbors应该是严格单调关系，那么任意一个neighbor w都不会使得SERIAL_1(w) = i.因此这一步选择的一定是 不相邻的节点. 当这一步完成之后，所有没有被selected vertex都一定有一个selected vertex作为它的neighbor.因此最终选择了一个2-ruling set.
- 原始V中的v到2-ruling set U中的u,一定有v->u的path
- - 如果p的selected vertex是它的succ，那么p被 pn 1-ruling
- - 如果p的selected vertex是它的prev, pp->p->pn，
- - - pn如果不被selected，pn一定被pnn selected,p 被pnn 2-ruling.

显然，这个过程可以在O(log n)时间内运行。初看似乎是需要O(n)个处理器来实现这个运行时间(只需分配一个processor到每个顶点v)，但实际上我们证明，实际上，这个时间是可以的
仅使用n/log n个处理器实现。要做到这一点，我们执行两个指令:

**Instruction1**:我们根据它们的SERIAL_1 number对顶点进行排序。排序的结果是每个顶点v将被赋予一个数字RANK(v)，1 =< RANK(v) =< n。没有两个顶点具有相同的RANK。


**Instruction2**：对于每个v, RANK(v) := RANK(v) + i * n / logn, 其中 i=SERIAL_1(v). （将vertex按照n/logn 系数划分）

这样我们就能在2logn rounds处理所有的vertices.在round j (1<= j <= 2logn)，我们处理所有使得 (j-1) n / logn < RANK(v) <= jn /logn的顶点v.

Instruction2可以保证，我们从不 同时处理两个 SERIAL_1  number 不同的顶点.

Instruction1只需要一个包含n个数的桶排序[0, log n - 1]. 本节的其余部分将展示如何在
O(log n)时间使用n/log n个处理器。我们注意到桶排序，虽然没有就地执行，但是只需要O(n)个空间。（阅读第3部分可能会有所帮助，关于prefix sum 并行算法）。

排序分三个阶段进行:我们对每个数字i进行计数SERIAL_1(v) = i的顶点数v，使用前缀求和
顺序算法中，我们在O(log n)时间内计算顶点v的个数SERIAL_1(v) < i。第三，对于每个顶点v，我们确定a唯一值RANK(v)，没有两个顶点的RANK是相同的。

Step1分为两个子阶段:
- Step1.1:我们划分这些顶点分成大小为log n的组。对于每一组，在O(log n)时间内，每个group使用一个Processor，我们计算使得SERIAL_1(v) =i 的顶点v的个数，0 <= i < logn.(我们也会动态地确定每一个顶点v，有多少顶点w，在v之前，满足SERIAL_1(w) =SERIAL_1(v).)我们得到n/logn个logn counts的group set
- Step1.2使用前缀和并行算法，对于log n中的每一个数字i，我们求和n/ log n个相关的count(对于每个i，每个group 1个计数)。显然，这一步，用n/log n processors在O(log n)时间内处理

Step2很简单

Step3中，对于每个顶点v，我们使用单个processor和O(1)时间计算RANK(v)，其中多个processor可以从相同的内存位置读取数据(这很容易使用n/log n个处理器，在O(log n)时间内模拟这个计算 使用一个EREW PRMAM)。RANK(v)将等于:
- 1，加上使得SERIAL_1(u)< SERIAL_1(v)的定点数(在第二阶段计算)，加上使SERIAL_1(w)= SERIAL_1(v)并且input array中w在v之前。The last number is obtained by adding the number of such vertices w that appear in groups prior to the group of v  and the number of such vertices w that appear prior to v in its own group.这两个数字都在第一个step中计算.

接下来就可以得出 对于log n个桶的桶排序算法，使用n/log n个处理器和O(log n)时间。

**Theorem2.3** 一个2-ruling set可以使用n/logn processors在O(logn)时间获取.

**Note**:为何不直接处理，因为graph可能分为不同的几个部分，不一定所有节点都在一个ring中.