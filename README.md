# Simulation de moteur physique

Projet C++ simulant les collisions entre différentes entités.

## Formules

### Détection des collisions

Pour détecter si une collision a eu lieu, le moteur vérifie selon quatre situations. Il retourne ensuite un objet de type ```Constraint``` contenant la normale $n$ et le point de contact $C$. On considère également $t$ la tangente, $P_1$ le centre du premier objet, et $P_2$ le centre du deuxième objet.

#### Sphère / sphère

Pour deux sphères de rayons $r_1$ et $r_2$, on a contact lorsque :

$$|P_2 - P_1| - r_1 - r_2 \le 0$$

Dans ce cas, on a donc :

$$n = \frac{P_2 - P_1}{|P_2 - P_1|}$$

Le point de contact est alors, au choix :

$$C = P_1 + r_1 n$$
$$C = P_2 - r_2 n$$

#### Box / box

A faire.

#### Box / sphère

Notons $w$ la longueur et $h$ la hauteur de la box. Notons $n_b$ sa normale et $t_b$ sa tangente. On peut calculer la distance horizontale et verticale de la sphère à la box avec :

$$w_s = (P_2 - P_1)t$$
$$h_s = (P_2 - P_1)n$$

Dans un premier temps, il y a potentiellement un contact lorsque :

$$|w_s| - r \le \frac{w}{2} \quad \text{ ou } \quad |h_s| - r \le \frac{h}{2}$$

Dans un deuxième temps, on vérifie également la distance aux quatre coins, qui est pour le moment comparée à $\sqrt{2}r > r$ :

$$\left| P_2 - \left( P_1 + \pm \frac{w}{2} t_b \pm \frac{h}{2} n_b \right) \right| - r \le 0$$

Pour déterminer le point de contact, on détecte le bord le plus proche en projetant la position actuelle de la sphère sur chacun des bords avec :

$$C_1 = P_1 + w_s t_b + \frac{h}{2} n_b, \quad n_1 = n_b$$
$$C_2 = P_1 + w_s t_b - \frac{h}{2} n_b, \quad n_2 = -n_b$$
$$C_3 = P_1 + \frac{w}{2} t_b + h_s n_b, \quad n_3 = t_b$$
$$C_4 = P_1 - \frac{w}{2} t_b + h_s n_b, \quad n_4 = -t_b$$

Et on récupère la normale et le point de contact en récupérant le point de contact le plus proche de la sphère :

$$m = \underset{i}{\arg\min{|P_2 - C_i|}}$$
$$n = n_m$$
$$C = C_m$$

#### Sphère / box

A faire.
