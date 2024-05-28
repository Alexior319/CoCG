(define (problem Kbox-4)
(:domain Kbox-4)
;;BestCase
(:init
(agent-at a1 p1-1)
(agent-at a2 p5-1)
(agent-at a3 p1-1)
(adj p1-1 p1-2)
(adj p2-1 p2-2)
(adj p3-1 p3-2)
(adj p4-1 p4-2)
(adj p5-1 p5-2)
(adj p1-2 p1-1)
(adj p2-2 p2-1)
(adj p3-2 p3-1)
(adj p4-2 p4-1)
(adj p5-2 p5-1)
(adj p1-1 p2-1)
(adj p2-1 p3-1)
(adj p3-1 p4-1)
(adj p4-1 p5-1)
(adj p2-1 p1-1)
(adj p3-1 p2-1)
(adj p4-1 p3-1)
(adj p5-1 p4-1)
(not (heavy b0))
(heavy b1)
(heavy b2)
(same-agent a1 a1)
(same-agent a2 a2)
(same-agent a3 a3)
(not (same-agent a1 a2))
(not (same-agent a2 a1))
(not (same-agent a1 a3))
(not (same-agent a3 a1))
(not (same-agent a3 a2))
(not (same-agent a2 a3))
(not (agent-at a1 p1-2))
(not (agent-at a1 p2-1))
(not (agent-at a1 p2-2))
(not (agent-at a1 p3-1))
(not (agent-at a1 p3-2))
(not (agent-at a1 p4-1))
(not (agent-at a1 p4-2))
(not (agent-at a1 p5-1))
(not (agent-at a1 p5-2))
(not (agent-at a2 p1-1))
(not (agent-at a2 p1-2))
(not (agent-at a2 p2-1))
(not (agent-at a2 p2-2))
(not (agent-at a2 p3-1))
(not (agent-at a2 p3-2))
(not (agent-at a2 p4-1))
(not (agent-at a2 p4-2))
(not (agent-at a2 p5-2))
(not (agent-at a3 p1-2))
(not (agent-at a3 p2-1))
(not (agent-at a3 p2-2))
(not (agent-at a3 p3-1))
(not (agent-at a3 p3-2))
(not (agent-at a3 p4-1))
(not (agent-at a3 p4-2))
(not (agent-at a3 p5-1))
(not (agent-at a3 p5-2))
(KNbox-at b0 p2-1)
(KNbox-at b0 p2-2)
(KNbox-at b0 p3-1)
(KNbox-at b0 p3-2)
(KNbox-at b0 p4-1)
(KNbox-at b0 p4-2)
(KNbox-at b0 p5-1)
(KNbox-at b0 p5-2)
(KNbox-at b1 p1-1)
(KNbox-at b1 p1-2)
(KNbox-at b1 p2-1)
(KNbox-at b1 p2-2)
(KNbox-at b1 p4-1)
(KNbox-at b1 p4-2)
(KNbox-at b1 p5-1)
(KNbox-at b1 p5-2)
(KNbox-at b2 p1-1)
(KNbox-at b2 p1-2)
(KNbox-at b2 p2-1)
(KNbox-at b2 p2-2)
(KNbox-at b2 p3-1)
(KNbox-at b2 p3-2)
(KNbox-at b2 p4-1)
(KNbox-at b2 p4-2)
(next-time t1 t2)
(next-time t2 t3)
(next-time t3 t4)
(next-time t4 t5)
(next-time t5 t6)
(next-time t6 t7)
(next-time t7 t8)
(next-time t8 t9)
(next-time t9 t10)
(next-time t10 t11)
(next-time t11 t12)
(next-time t12 t13)
(next-time t13 t14)
(next-time t14 t15)
(next-time t15 t16)
(next-time t16 t17)
(next-time t17 t18)
(next-time t18 t19)
(next-time t19 t20)
(next-time t20 t21)
(next-time t21 t22)
(next-time t22 t23)
(next-time t23 t24)
(next-time t24 t25)
(next-time t25 t26)
(next-time t26 t27)
(next-time t27 t28)
(next-time t28 t29)
(next-time t29 t30)
(next-time t30 t31)
(next-time t31 t32)
(next-time t32 t33)
(next-time t33 t34)
(next-time t34 t35)
(next-time t35 t36)
(next-time t36 t37)
(next-time t37 t38)
(next-time t38 t39)
(next-time t39 t40)
(next-time t40 t41)
(next-time t41 t42)
(next-time t42 t43)
(next-time t43 t44)
(next-time t44 t45)
(next-time t45 t46)
(next-time t46 t47)
(next-time t47 t48)
(next-time t48 t49)
(next-time t49 t50)
(next-time t50 t51)
(next-time t51 t52)
(next-time t52 t53)
(next-time t53 t54)
(next-time t54 t55)
(next-time t55 t56)
(next-time t56 t57)
(next-time t57 t58)
(next-time t58 t59)
(next-time t59 t60)
(next-time t60 t61)
(next-time t61 t62)
(next-time t62 t63)
(next-time t63 t64)
(next-time t64 t65)
(next-time t65 t66)
(next-time t66 t67)
(next-time t67 t68)
(next-time t68 t69)
(next-time t69 t70)
(next-time t70 t71)
(next-time t71 t72)
(next-time t72 t73)
(next-time t73 t74)
(next-time t74 t75)
(next-time t75 t76)
(next-time t76 t77)
(next-time t77 t78)
(next-time t78 t79)
(next-time t79 t80)
(next-time t80 t81)
(next-time t81 t82)
(next-time t82 t83)
(next-time t83 t84)
(next-time t84 t85)
(next-time t85 t86)
(next-time t86 t87)
(next-time t87 t88)
(next-time t88 t89)
(next-time t89 t90)
(next-time t90 t91)
(next-time t91 t92)
(next-time t92 t93)
(next-time t93 t94)
(next-time t94 t95)
(next-time t95 t96)
(next-time t96 t97)
(next-time t97 t98)
(next-time t98 t99)
(next-time t99 t100)
(next-time t100 t101)
(next-time t101 t102)
(next-time t102 t103)
(next-time t103 t104)
(next-time t104 t105)
(next-time t105 t106)
(next-time t106 t107)
(next-time t107 t108)
(next-time t108 t109)
(next-time t109 t110)
(next-time t110 t111)
(next-time t111 t112)
(next-time t112 t113)
(next-time t113 t114)
(next-time t114 t115)
(next-time t115 t116)
(next-time t116 t117)
(next-time t117 t118)
(next-time t118 t119)
(next-time t119 t120)
(next-time t120 t121)
(next-time t121 t122)
(next-time t122 t123)
(next-time t123 t124)
(next-time t124 t125)
(next-time t125 t126)
(next-time t126 t127)
(next-time t127 t128)
(next-time t128 t129)
(next-time t129 t130)
(next-time t130 t131)
(next-time t131 t132)
(next-time t132 t133)
(next-time t133 t134)
(next-time t134 t135)
(next-time t135 t136)
(next-time t136 t137)
(next-time t137 t138)
(next-time t138 t139)
(next-time t139 t140)
(next-time t140 t141)
(next-time t141 t142)
(next-time t142 t143)
(next-time t143 t144)
(next-time t144 t145)
(next-time t145 t146)
(next-time t146 t147)
(next-time t147 t148)
(next-time t148 t149)
(next-time t149 t150)
(next-time t150 t151)
(next-time t151 t152)
(next-time t152 t153)
(next-time t153 t154)
(next-time t154 t155)
(next-time t155 t156)
(next-time t156 t157)
(next-time t157 t158)
(next-time t158 t159)
(next-time t159 t160)
(next-time t160 t161)
(next-time t161 t162)
(next-time t162 t163)
(next-time t163 t164)
(next-time t164 t165)
(next-time t165 t166)
(next-time t166 t167)
(next-time t167 t168)
(next-time t168 t169)
(next-time t169 t170)
(next-time t170 t171)
(next-time t171 t172)
(next-time t172 t173)
(next-time t173 t174)
(next-time t174 t175)
(next-time t175 t176)
(next-time t176 t177)
(next-time t177 t178)
(next-time t178 t179)
(next-time t179 t180)
(next-time t180 t181)
(next-time t181 t182)
(next-time t182 t183)
(next-time t183 t184)
(next-time t184 t185)
(next-time t185 t186)
(next-time t186 t187)
(next-time t187 t188)
(next-time t188 t189)
(next-time t189 t190)
(next-time t190 t191)
(next-time t191 t192)
(next-time t192 t193)
(next-time t193 t194)
(next-time t194 t195)
(next-time t195 t196)
(next-time t196 t197)
(next-time t197 t198)
(next-time t198 t199)
(next-time t199 t200)
(next-time t200 t200)
(current-time t1)
(active-agent a3)
)
(:goal 
(and (Kbox-at b0 p1-2) (Kbox-at b1 p3-2) (Kbox-at b2 p5-2)))
)
