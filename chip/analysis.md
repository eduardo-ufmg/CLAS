MIDPOINT - Xj
| CLUSTER -1 | CLUSTER 1 | RESULT  |
|------------|-----------|---------|
| -1, -1     | -1, 1     | CORRECT |
| -1, -1     | 1, -1     | CORRECT |
| -1, -1     | 1, 1      | OPPOSITE|
| -1, 1      | -1, -1    | CORRECT |
| -1, 1      | 1, -1     | CORRECT |
| -1, 1      | 1, 1      | OPPOSITE|
| 1, -1      | -1, -1    | OPPOSITE|
| 1, -1      | -1, 1     | MIXED I | 
| 1, -1      | 1, 1      | OPPOSITE|
| 1, 1       | -1, -1    | OPPOSITE|
| 1, 1       | -1, 1     | OPPOSITE|
| 1, 1       | 1, -1     | CORRECT |

Xi - Xj
| CLUSTER -1 | CLUSTER 1 | RESULT             |
|------------|-----------|--------------------|
| -1, -1     | -1, 1     | OPPOSITE           |
| -1, -1     | 1, -1     | OPPOSITE           |
| -1, -1     | 1, 1      | CORRECT            |
| -1, 1      | -1, -1    | CORRECT            |
| -1, 1      | 1, -1     | CORRECT            |
| -1, 1      | 1, 1      | OPPOSITE, MIXED IV |
| 1, -1      | -1, -1    | CORRECT            |
| 1, -1      | -1, 1     | OPPOSITE           |
| 1, -1      | 1, 1      | CORRECT            |
| 1, 1       | -1, -1    | CORRECT            |
| 1, 1       | -1, 1     | OPPOSITE           |
| 1, 1       | 1, -1     | CORRECT            |

abs(MIDPOINT - Xj)
| CLUSTER -1 | CLUSTER 1 | RESULT   |
|------------|-----------|----------|
| -1, -1     | -1, 1     | CORRECT  |
| -1, -1     | 1, -1     | CORRECT  |
| -1, -1     | 1, 1      | CORRECT  |
| -1, 1      | -1, -1    | OPPOSITE |
| -1, 1      | 1, -1     | MIXED II |
| -1, 1      | 1, 1      | CORRECT  |
| 1, -1      | -1, -1    | OPPOSITE |
| 1, -1      | -1, 1     | MIXED III|
| 1, -1      | 1, 1      | CORRECT  |
| 1, 1       | -1, -1    | OPPOSITE |
| 1, 1       | -1, 1     | OPPOSITE |
| 1, 1       | 1, -1     | OPPOSITE |

abs(Xi - Xj)
| CLUSTER -1 | CLUSTER 1 | RESULT   |
|------------|-----------|----------|
| -1, -1     | -1, 1     | CORRECT  |
| -1, -1     | 1, -1     | CORRECT  |
| -1, -1     | 1, 1      | CORRECT  |
| -1, 1      | -1, -1    | OPPOSITE |
| -1, 1      | 1, -1     | MIXED II |
| -1, 1      | 1, 1      | CORRECT  |
| 1, -1      | -1, -1    | OPPOSITE |
| 1, -1      | -1, 1     | MIXED III|
| 1, -1      | 1, 1      | CORRECT  |
| 1, 1       | -1, -1    | OPPOSITE |
| 1, 1       | -1, 1     | OPPOSITE |
| 1, 1       | 1, -1     | OPPOSITE |

Xi + Xj
| CLUSTER -1 | CLUSTER 1 | RESULT           |
|------------|-----------|------------------|
| -1, -1     | -1, 1     | ALL 1            |
| -1, -1     | 1, -1     | ALL -1           |
| -1, -1     | 1, 1      | MIXED III, ALL 1 |
| -1, 1      | -1, -1    | ALL 1            |
| -1, 1      | 1, -1     | OPPOSITE         |
| -1, 1      | 1, 1      | ALL 1            |
| 1, -1      | -1, -1    | ALL 1            |
| 1, -1      | -1, 1     | OPPOSITE         |
| 1, -1      | 1, 1      | ALL -1           |
| 1, 1       | -1, -1    | CORRECT          |
| 1, 1       | -1, 1     | ALL -1           |
| 1, 1       | 1, -1     | ALL 1            |

MIXED I: Vertices for which `x > -y` are correctly labeled, others are labeled as the opposite cluster.
MIXED II: Vertices in `(x < y Δ x > -y)` are correctly labeled, others are labeled as the opposite cluster.
MIXED III: Vertices in `(x < y Δ x < -y)` are correctly labeled, others are labeled as the opposite cluster.
MIEXD IV: Vertices for which `y < 1` are correctly labeled, others are labeled as the opposite cluster.